/**
 * Jingga
 *
 * @copyright 2023, Dominic Szablewski - https://phoboslab.org
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_AUDIO_QOA_H
#define TOS_AUDIO_QOA_H

#include "../stdlib/Types.h"
#include "../utils/EndianUtils.h"
#include "../audio/Audio.cpp"
#include "../stdlib/Simd.h"

#define QOA_SLICE_LEN 20
#define QOA_SLICES_PER_FRAME 256
#define QOA_FRAME_LEN (QOA_SLICES_PER_FRAME * QOA_SLICE_LEN)
#define QOA_LMS_LEN 4
#define QOA_MAX_CHANNELS 8

#define QOA_FRAME_SIZE(channels, slices) (4 + QOA_LMS_LEN * 4 * (channels) + 8 * (slices) * (channels))

struct QoaLms {
	int32_4 history; // automatically QOA_LMS_LEN size
	int32_4 weights; // automatically QOA_LMS_LEN size
};

/*
The quant_tab provides an index into the dequant_tab for residuals in the
range of -8 .. 8. It maps this range to just 3bits and becomes less accurate at
the higher end. Note that the residual zero is identical to the lowest positive
value. This is mostly fine, since the qoa_div() function always rounds away
from zero.
*/
static const int32 qoa_quant_tab[17] = {
	7, 7, 7, 5, 5, 3, 3, 1, /* -8..-1 */
	0,                      /*  0     */
	0, 2, 2, 4, 4, 6, 6, 6  /*  1.. 8 */
};

/*
We have 16 different scalefactors. Like the quantized residuals these become
less accurate at the higher end. In theory, the highest scalefactor that we
would need to encode the highest 16bit residual is (2**16)/8 = 8192. However we
rely on the LMS filter to predict samples accurately enough that a maximum
residual of one quarter of the 16 bit range is sufficient. I.e. with the
scalefactor 2048 times the quant range of 8 we can encode residuals up to 2**14.

The scalefactor values are computed as:
scalefactor_tab[s] <- round(pow(s + 1, 2.75))
*/
static const int32 qoa_scalefactor_tab[16] = {
	1, 7, 21, 45, 84, 138, 211, 304, 421, 562, 731, 928, 1157, 1419, 1715, 2048
};


/*
The reciprocal_tab maps each of the 16 scalefactors to their rounded
reciprocals 1/scalefactor. This allows us to calculate the scaled residuals in
the encoder with just one multiplication instead of an expensive division. We
do this in .16 fixed point with integers, instead of floats.

The reciprocal_tab is computed as:
reciprocal_tab[s] <- ((1<<16) + scalefactor_tab[s] - 1) / scalefactor_tab[s]
*/
static const int32 qoa_reciprocal_tab[16] = {
	65536, 9363, 3121, 1457, 781, 475, 311, 216, 156, 117, 90, 71, 57, 47, 39, 32
};

/*
The dequant_tab maps each of the scalefactors and quantized residuals to
their unscaled & dequantized version.

Since qoa_div rounds away from the zero, the smallest entries are mapped to 3/4
instead of 1. The dequant_tab assumes the following dequantized values for each
of the quant_tab indices and is computed as:
float dqt[8] = {0.75, -0.75, 2.5, -2.5, 4.5, -4.5, 7, -7};
dequant_tab[s][q] <- round_ties_away_from_zero(scalefactor_tab[s] * dqt[q])

The rounding employed here is "to nearest, ties away from zero",  i.e. positive
and negative values are treated symmetrically.
*/
static const int32 qoa_dequant_tab[16][8] = {
	{   1,    -1,    3,    -3,    5,    -5,     7,     -7},
	{   5,    -5,   18,   -18,   32,   -32,    49,    -49},
	{  16,   -16,   53,   -53,   95,   -95,   147,   -147},
	{  34,   -34,  113,  -113,  203,  -203,   315,   -315},
	{  63,   -63,  210,  -210,  378,  -378,   588,   -588},
	{ 104,  -104,  345,  -345,  621,  -621,   966,   -966},
	{ 158,  -158,  528,  -528,  950,  -950,  1477,  -1477},
	{ 228,  -228,  760,  -760, 1368, -1368,  2128,  -2128},
	{ 316,  -316, 1053, -1053, 1895, -1895,  2947,  -2947},
	{ 422,  -422, 1405, -1405, 2529, -2529,  3934,  -3934},
	{ 548,  -548, 1828, -1828, 3290, -3290,  5117,  -5117},
	{ 696,  -696, 2320, -2320, 4176, -4176,  6496,  -6496},
	{ 868,  -868, 2893, -2893, 5207, -5207,  8099,  -8099},
	{1064, -1064, 3548, -3548, 6386, -6386,  9933,  -9933},
	{1286, -1286, 4288, -4288, 7718, -7718, 12005, -12005},
	{1536, -1536, 5120, -5120, 9216, -9216, 14336, -14336},
};


/*
The Least Mean Squares Filter is the heart of QOA. It predicts the next
sample based on the previous 4 reconstructed samples. It does so by continuously
adjusting 4 weights based on the residual of the previous prediction.

The next sample is predicted as the sum of (weight[i] * history[i]).

The adjustment of the weights is done with a "Sign-Sign-LMS" that adds or
subtracts the residual to each weight, based on the corresponding sample from
the history. This, surprisingly, is sufficient to get worthwhile predictions.

This is all done with fixed point integers. Hence the right-shifts when updating
the weights and calculating the prediction.
*/
static inline
int32 qoa_lms_predict(QoaLms* lms)
{
    __m128i products = _mm_mullo_epi32(lms->weights.s, lms->history.s);
    __m128i sum1 = _mm_hadd_epi32(products, products);
    __m128i sum2 = _mm_hadd_epi32(sum1, sum1);

    int32 prediction = _mm_cvtsi128_si32(sum2);

    return prediction >> 13;
}

static inline
void qoa_lms_update(QoaLms* lms, int32 sample, int32 residual) {
	int32 delta = residual >> 4;

    __m128i delta_vec = _mm_set1_epi32(delta);
    __m128i zero_vec = _mm_setzero_si128();

    // Calculate adjustments for weights based on the sign of history
    __m128i sign_mask = _mm_cmpgt_epi32(zero_vec, lms->history.s); // history < 0
    __m128i delta_adjust = _mm_blendv_epi8(delta_vec, _mm_sub_epi32(zero_vec, delta_vec), sign_mask);

    // Update weights
    lms->weights.s = _mm_add_epi32(lms->weights.s, delta_adjust);

    // Shift history left
    lms->history.s = _mm_alignr_epi8(lms->history.s, lms->history.s, 4); // Shift left by 1 int32 (4 bytes)

    // Insert the new sample into the last position of history
    lms->history.s = _mm_insert_epi32(lms->history.s, sample, QOA_LMS_LEN - 1);

    // Update the last weight based on the sign of the new sample
    int32 sample_sign_adjust = (sample < 0) ? -delta : delta;
    lms->weights.s = _mm_insert_epi32(
        lms->weights.s,
        _mm_extract_epi32(lms->weights.s, QOA_LMS_LEN - 1) + sample_sign_adjust,
        QOA_LMS_LEN - 1
    );
}

/*
qoa_div() implements a rounding division, but avoids rounding to zero for
small numbers. E.g. 0.1 will be rounded to 1. Note that 0 itself still
returns as 0, which is handled in the qoa_quant_tab[].
qoa_div() takes an index into the .16 fixed point qoa_reciprocal_tab as an
argument, so it can do the division with a cheaper integer multiplication.
*/
static inline
int32 qoa_div(int32 v, int32 scalefactor) {
	int32 reciprocal = qoa_reciprocal_tab[scalefactor];
	int32 n = (v * reciprocal + (1 << 15)) >> 16;

    /* round away from 0 */
	n = n + ((v > 0) - (v < 0)) - ((n > 0) - (n < 0));

	return n;
}

static inline
int32 qoa_clamp(int32 v, int32 min, int32 max) {
	if (v < min) { return min; }
	if (v > max) { return max; }
	return v;
}

/*
This specialized clamp function for the signed 16 bit range improves decode
performance quite a bit. The extra if() statement works nicely with the CPUs
branch prediction as this branch is rarely taken.
*/
static inline
int32 qoa_clamp_s16(int32 v) {
	if ((uint32) (v + 32768) > 65535) {
		if (v < -32768) { return -32768; }
		if (v >  32767) { return  32767; }
	}

	return v;
}

uint32 qoa_encode_frame(const int16* sample_data, int32 channels, uint32 frame_samples, QoaLms* lms, byte* bytes)
{
    byte* start = bytes;

	int32 prev_scalefactor[QOA_MAX_CHANNELS] = {0};

	// Write the frame header
    *((uint32 *) bytes) = SWAP_ENDIAN_LITTLE(frame_samples);
    bytes += sizeof(frame_samples);

	for (uint32 c = 0; c < channels; ++c) {
		// Load the history and weights as 16-bit masked values
        __m128i history = _mm_and_si128(lms[c].history.s, _mm_set1_epi32(0xFFFF));
        __m128i weights = _mm_and_si128(lms[c].weights.s, _mm_set1_epi32(0xFFFF));

        // Permute and pack 16-bit values into 64-bit results
        __m128i packed_history = _mm_packus_epi32(history, history); // Pack 16-bit values
        __m128i packed_weights = _mm_packus_epi32(weights, weights);

        // Shuffle packed values into the correct order for 64-bit construction
        __m128i history_64 = _mm_packus_epi16(packed_history, packed_history);
        __m128i weights_64 = _mm_packus_epi16(packed_weights, packed_weights);

        // Extract 64-bit results and swap endian if necessary
        uint64 final_history = _mm_extract_epi64(history_64, 0);
        uint64 final_weights = _mm_extract_epi64(weights_64, 0);

        // Store results with endian swap
        *((uint64*) bytes) = SWAP_ENDIAN_LITTLE(final_history);
        bytes += sizeof(final_history);

        *((uint64*) bytes) = SWAP_ENDIAN_LITTLE(final_weights);
        bytes += sizeof(final_weights);
	}

	/*
    We encode all samples with the channels interleaved on a slice level.
	E.g. for stereo: (ch-0, slice 0), (ch 1, slice 0), (ch 0, slice 1), ...
    */
	for (uint32 sample_index = 0; sample_index < frame_samples; sample_index += QOA_SLICE_LEN) {
        // @performance SIMDable
		for (uint32 c = 0; c < channels; ++c) {
			int32 slice_len = qoa_clamp(QOA_SLICE_LEN, 0, frame_samples - sample_index);
			int32 slice_start = sample_index * channels + c;
			int32 slice_end = (sample_index + slice_len) * channels + c;

			/*
            Brute for search for the best scalefactor. Just go through all
			16 scalefactors, encode all samples for the current slice and
			meassure the total squared error.
            */
			uint64 best_rank = -1;
			uint64 best_slice = 0;
			QoaLms best_lms;
			int32 best_scalefactor = 0;

			for (int32 sfi = 0; sfi < 16; ++sfi) {
				/*
                There is a strong correlation between the scalefactors of
				neighboring slices. As an optimization, start testing
				the best scalefactor of the previous slice first.
                */
				int32 scalefactor = (sfi + prev_scalefactor[c]) % 16;

				/*
                We have to reset the LMS state to the last known good one
				before trying each scalefactor, as each pass updates the LMS
				state when encoding.
                */
				QoaLms lms_temp = lms[c];
				uint64 slice = scalefactor;
				uint64 current_rank = 0;

				for (int32 si = slice_start; si < slice_end; si += channels) {
					int32 sample = sample_data[si];
					int32 predicted = qoa_lms_predict(&lms_temp);

					int32 residual = sample - predicted;
					int32 scaled = qoa_div(residual, scalefactor);
					int32 clamped = qoa_clamp(scaled, -8, 8);
					int32 quantized = qoa_quant_tab[clamped + 8];
					int32 dequantized = qoa_dequant_tab[scalefactor][quantized];
					int32 reconstructed = qoa_clamp_s16(predicted + dequantized);

					/*
                    If the weights have grown too large, we introduce a penalty
					here. This prevents pops/clicks in certain problem cases
                    */
                    // Compute weights squared: w^2
                    __m128i weights_squared = _mm_mullo_epi32(lms_temp.weights.s, lms_temp.weights.s);

                    // Perform horizontal addition to sum all squared weights
                    __m128i sum1 = _mm_hadd_epi32(weights_squared, weights_squared);
                    __m128i sum2 = _mm_hadd_epi32(sum1, sum1);

                    // Extract the final sum (scalar)
                    int32 sum_of_squares = _mm_cvtsi128_si32(sum2);

                    // Apply the shift and subtraction
                    int32 weights_penalty = (sum_of_squares >> 18) - 0x8FF;
					if (weights_penalty < 0) {
						weights_penalty = 0;
					}

					int64 error = (sample - reconstructed);
					uint64 error_sq = error * error;

					current_rank += error_sq + weights_penalty * weights_penalty;
					if (current_rank > best_rank) {
						break;
					}

					qoa_lms_update(&lms_temp, reconstructed, dequantized);
					slice = (slice << 3) | quantized;
				}

				if (current_rank < best_rank) {
					best_rank = current_rank;
					best_slice = slice;
					best_lms = lms_temp;
					best_scalefactor = scalefactor;
				}
			}

			prev_scalefactor[c] = best_scalefactor;

			lms[c] = best_lms;

			/*
            If this slice was shorter than QOA_SLICE_LEN, we have to left-
			shift all encoded data, to ensure the rightmost bits are the empty
			ones. This should only happen in the last frame of a file as all
			slices are completely filled otherwise.
            */
			best_slice <<= (QOA_SLICE_LEN - slice_len) * 3;

            *((uint64 *) bytes) = SWAP_ENDIAN_LITTLE(best_slice);
            bytes += sizeof(best_slice);
		}
	}

	return (uint32) (bytes - start);
}

uint32 qoa_encode(const Audio* audio, byte* data)
{
    byte* start = data;

	/* Calculate the encoded size and allocate */
    uint32 sample_count = audio->size / (audio->channels * audio->bloc_size);
	uint32 num_frames = (sample_count + QOA_FRAME_LEN - 1) / QOA_FRAME_LEN;
	uint32 num_slices = (sample_count + QOA_SLICE_LEN - 1) / QOA_SLICE_LEN;

    QoaLms lms[QOA_MAX_CHANNELS];
    __m128i weights_init = _mm_set_epi32(1 << 14, -(1 << 13), 0, 0);
    __m128i history_init = _mm_setzero_si128();

    for (int32 i = 0; i < audio->channels; ++i) {
        /*
        Set the initial LMS weights to {0, 0, -1, 2}. This helps with the
        prediction of the first few ms of a file.
        */
        lms[i].weights.s = weights_init;
        lms[i].history.s = history_init;
    }

	// Go through all frames
	int32 frame_samples = QOA_FRAME_LEN;
    int32 p = 0;

	for (uint32 sample_index = 0; sample_index < sample_count; sample_index += frame_samples) {
		frame_samples = qoa_clamp(QOA_FRAME_LEN, 0, sample_count - sample_index);
		data += qoa_encode_frame(
            (int16 *) (audio->data + sample_index * audio->channels * audio->bloc_size),
            audio->channels, frame_samples, lms, data
        );
	}

	return (uint32) (data - start);
}

uint32 qoa_decode_frame(const byte* bytes, int32 channels, QoaLms* lms, byte* sample_data)
{
    const byte* start = bytes;

	// Read and verify the frame header
    uint32 frame_samples = SWAP_ENDIAN_LITTLE(*((uint32 *) bytes));
    bytes += sizeof(frame_samples);

    uint32 slices = (frame_samples + QOA_SLICE_LEN - 1) / QOA_SLICE_LEN;
    uint32 frame_size = QOA_FRAME_SIZE(channels, slices);
	uint32 data_size = frame_size - 4 - QOA_LMS_LEN * 4 * channels;
	uint32 num_slices = data_size / 8;
	uint32 max_total_samples = num_slices * QOA_SLICE_LEN;

	// Read the LMS state: 4 x 2 bytes history, 4 x 2 bytes weights per channel
	for (uint32 c = 0; c < channels; ++c) {
		uint64 history = SWAP_ENDIAN_LITTLE(*((uint64 *) bytes));
        bytes += sizeof(history);

		uint64 weights = SWAP_ENDIAN_LITTLE(*((uint64 *) bytes));
        bytes += sizeof(weights);

        alignas(16) int32 history_array[4];
        alignas(16) int32 weights_array[4];

		for (int32 i = 0; i < QOA_LMS_LEN; ++i) {
			history_array[i] = ((int16) (history >> 48));
			history <<= 16;

			weights_array[i] = ((int16) (weights >> 48));
			weights <<= 16;
		}

        lms[c].history.s = _mm_set_epi32(history_array[3], history_array[2], history_array[1], history_array[0]);
        lms[c].weights.s = _mm_set_epi32(weights_array[3], weights_array[2], weights_array[1], weights_array[0]);
	}

	// Decode all slices for all channels in this frame
	for (uint32 sample_index = 0; sample_index < frame_samples; sample_index += QOA_SLICE_LEN) {
		for (uint32 c = 0; c < channels; c++) {
			uint64 slice = SWAP_ENDIAN_LITTLE(*((uint64 *) bytes));
            bytes += sizeof(slice);

			int32 scalefactor = (slice >> 60) & 0xf;
			slice <<= 4;

			int32 slice_start = sample_index * channels + c;
			int32 slice_end = qoa_clamp(sample_index + QOA_SLICE_LEN, 0, frame_samples) * channels + c;

			for (int32 si = slice_start; si < slice_end; si += channels) {
				int32 predicted = qoa_lms_predict(&lms[c]);
				int32 quantized = (slice >> 61) & 0x7;
				int32 dequantized = qoa_dequant_tab[scalefactor][quantized];
				int32 reconstructed = qoa_clamp_s16(predicted + dequantized);

				sample_data[si] = reconstructed;
				slice <<= 3;

				qoa_lms_update(&lms[c], reconstructed, dequantized);
			}
		}
	}

	return (uint32) (bytes - start);
}


uint32 qoa_decode(const byte* data, Audio* audio)
{
    uint32 header_length = audio_header_from_data(data, audio);
    uint32 p = header_length;
	uint32 frame_size;
    byte* sample_ptr = audio->data;

    QoaLms lms[QOA_MAX_CHANNELS];

    uint32 limit = 4 + QOA_LMS_LEN * 4 * audio->channels;

	do {
		frame_size = qoa_decode_frame(data + p, audio->channels, lms, sample_ptr);
        sample_ptr += frame_size;
		p += frame_size;
	} while (frame_size && audio->size - p >= limit);
    // @question do we really need the audio->size - p >= limit check or would p < audio->size be sufficient?

	return header_length + audio->size;
}

#endif