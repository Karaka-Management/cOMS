// Remarks: sizes for the second matrix/vector are often implied by the first parameter and the rules for matrix/vector multiplication.

/////////////////////////////////
// Multiplication
/////////////////////////////////

// Array vector multiplication
/////////////////////////////////
// mult_vec_int32(int64_t *a, size_t a, int32_t *b)
// mult_vec_int64(int64_t *a, size_t a, int64_t *b)
// mult_vec_float(int64_t *a, size_t a, float *b)

// mult_scal_int32(int64_t *a, size_t a, int32_t b)
// mult_scal_int64(int64_t *a, size_t a, int64_t b)
// mult_scal_float(int64_t *a, size_t a, float b)

// Vector 2 vector multiplication
/////////////////////////////////
// v2_mult_vec_int32(v2 *a, int32_t *b)
// v2_mult_vec_int64(v2 *a, int64_t *b)
// v2_mult_vec_float(v2 *a, float *b)
// v2_mult_vec_v2(v2 *a, v2 *b)

// v2_mult_scal_int32(v2 *a, int32_t b)
// v2_mult_scal_int64(v2 *a, int64_t b)
// v2_mult_scal_float(v2 *a, float b)

// Vector 3 vector multiplication
/////////////////////////////////
// v3_mult_vec_int32(v3 *a, int32_t *b)
// v3_mult_vec_int64(v3 *a, int64_t *b)
// v3_mult_vec_float(v3 *a, float *b)
// v3_mult_vec_v3(v3 *a, v3 *b)

// v3_mult_scal_int32(v3 *a, int32_t b)
// v3_mult_scal_int64(v3 *a, int64_t b)
// v3_mult_scal_float(v3 *a, float b)

// Vector 4 vector multiplication
/////////////////////////////////
// v4_mult_vec_int32(v4 *a, int32_t *b)
// v4_mult_vec_int64(v4 *a, int64_t *b)
// v4_mult_vec_float(v4 *a, float *b)
// v4_mult_vec_v4(v4 *a, v4 *b)

// v4_mult_scal_int32(v4 *a, int32_t b)
// v4_mult_scal_int64(v4 *a, int64_t b)
// v4_mult_scal_float(v4 *a, float b)

/////////////////////////////////
// Addition
/////////////////////////////////

// Array vector addition
/////////////////////////////////
// add_vec_int32(int64_t *a, size_t a, int32_t *b)
// add_vec_int64(int64_t *a, size_t a, int64_t *b)
// add_vec_float(int64_t *a, size_t a, float *b)

// add_scal_int32(int64_t *a, size_t a, int32_t b)
// add_scal_int64(int64_t *a, size_t a, int64_t b)
// add_scal_float(int64_t *a, size_t a, float b)

// Vector 2 vector addition
/////////////////////////////////
// v2_add_vec_int32(v2 *a, int32_t *b)
// v2_add_vec_int64(v2 *a, int64_t *b)
// v2_add_vec_float(v2 *a, float *b)
// v2_add_vec_v2(v2 *a, v2 *b)

// v2_add_scal_int32(v2 *a, int32_t b)
// v2_add_scal_int64(v2 *a, int64_t b)
// v2_add_scal_float(v2 *a, float b)

// Vector 3 vector addition
/////////////////////////////////
// v3_add_vec_int32(v3 *a, int32_t *b)
// v3_add_vec_int64(v3 *a, int64_t *b)
// v3_add_vec_float(v3 *a, float *b)
// v3_add_vec_v3(v3 *a, v3 *b)

// v3_add_scal_int32(v3 *a, int32_t b)
// v3_add_scal_int64(v3 *a, int64_t b)
// v3_add_scal_float(v3 *a, float b)

// Vector 4 vector addition
/////////////////////////////////
// v4_add_vec_int32(v4 *a, int32_t *b)
// v4_add_vec_int64(v4 *a, int64_t *b)
// v4_add_vec_float(v4 *a, float *b)
// v4_add_vec_v4(v4 *a, v4 *b)

// v4_add_scal_int32(v4 *a, int32_t b)
// v4_add_scal_int64(v4 *a, int64_t b)
// v4_add_scal_float(v4 *a, float b)

/////////////////////////////////
// Subtraction
/////////////////////////////////

// Array vector subtraction
/////////////////////////////////
// sub_vec_int32(int64_t *a, size_t a, int32_t *b)
// sub_vec_int64(int64_t *a, size_t a, int64_t *b)
// sub_vec_float(int64_t *a, size_t a, float *b)

// sub_scal_int32(int64_t *a, size_t a, int32_t b)
// sub_scal_int64(int64_t *a, size_t a, int64_t b)
// sub_scal_float(int64_t *a, size_t a, float b)

// Vector 2 vector subtraction
/////////////////////////////////
// v2_sub_vec_int32(v2 *a, int32_t *b)
// v2_sub_vec_int64(v2 *a, int64_t *b)
// v2_sub_vec_float(v2 *a, float *b)
// v2_sub_vec_v2(v2 *a, v2 *b)

// v2_sub_scal_int32(v2 *a, int32_t b)
// v2_sub_scal_int64(v2 *a, int64_t b)
// v2_sub_scal_float(v2 *a, float b)

// Vector 3 vector subtraction
/////////////////////////////////
// v3_sub_vec_int32(v3 *a, int32_t *b)
// v3_sub_vec_int64(v3 *a, int64_t *b)
// v3_sub_vec_float(v3 *a, float *b)
// v3_sub_vec_v3(v3 *a, v3 *b)

// v3_sub_scal_int32(v3 *a, int32_t b)
// v3_sub_scal_int64(v3 *a, int64_t b)
// v3_sub_scal_float(v3 *a, float b)

// Vector 4 vector subtraction
/////////////////////////////////
// v4_sub_vec_int32(v4 *a, int32_t *b)
// v4_sub_vec_int64(v4 *a, int64_t *b)
// v4_sub_vec_float(v4 *a, float *b)
// v4_sub_vec_v4(v4 *a, v4 *b)

// v4_sub_scal_int32(v4 *a, int32_t b)
// v4_sub_scal_int64(v4 *a, int64_t b)
// v4_sub_scal_float(v4 *a, float b)

/////////////////////////////////
// Other
/////////////////////////////////

// Cross product
/////////////////////////////////
// cross_int32(int64_t *a, size_t a, int32_t *b)
// cross_int64(int64_t *a, size_t a, int64_t *b)
// cross_float(int64_t *a, size_t a, float *b)

// v2_cross_v2(v2 *a, v2 *b)
// v3_cross_v3(v3 *a, v3 *b)
// v4_cross_v4(v4 *a, v4 *b)

// Dot product
/////////////////////////////////
// dot_int32(int64_t *a, size_t a, int32_t *b)
// dot_int64(int64_t *a, size_t a, int64_t *b)
// dot_float(int64_t *a, size_t a, float *b)

// v2_dot_v2(v2 *a, v2 *b)
// v3_dot_v3(v3 *a, v3 *b)
// v4_dot_v4(v4 *a, v4 *b)

// Angle
/////////////////////////////////
// angle_int32(int64_t *a, size_t a, int32_t *b)
// angle_int64(int64_t *a, size_t a, int64_t *b)
// angle_float(int64_t *a, size_t a, float *b)

// v2_angle_v2(v2 *a, v2 *b)
// v3_angle_v3(v3 *a, v3 *b)
// v4_angle_v4(v4 *a, v4 *b)

// Cosine
/////////////////////////////////
// cosine_int32(int64_t *a, size_t a, int32_t *b)
// cosine_int64(int64_t *a, size_t a, int64_t *b)
// cosine_float(int64_t *a, size_t a, float *b)

// v2_cosine_v2(v2 *a, v2 *b)
// v3_cosine_v3(v3 *a, v3 *b)
// v4_cosine_v4(v4 *a, v4 *b)