/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_IMAGE_DEFAULT_COLORS_H
#define COMS_IMAGE_DEFAULT_COLORS_H

#include "../stdlib/Types.h"

// @todo some of the colors are bad
// @todo basically all the dark colors could be made darker -> space out some of the very bright colors a little bit more (see 3rd - 5th column)

const uint32 default_colors_256[256] = {
            0xFFFFFF00, 0xFFFFFFFF, 0xE9E9E9FF, 0xD9D9D9FF, 0xC9C9C9FF, 0xB9B9B9FF, 0xA9A9A9FF, 0x999999FF, 0x898989FF, 0x797979FF, 0x696969FF, 0x595959FF, 0x494949FF, 0x393939FF, 0x1F1F1FFF, 0x000000FF,
            0xF6D3CFFF, 0xF8C2BDFF, 0xFB9E91FF, 0xFC8B7EFF, 0xFC796CFF, 0xFD675BFF, 0xFF554BFF, 0xFF4340FF, 0xF82D2EFF, 0xF6171BFF, 0xE60412FF, 0xCC0004FF, 0xA70004FF, 0x7A0306FF, 0x4f0107FF, 0x330004FF,
            0xFDDDCEFF, 0xFCCFB6FF, 0xFDB69EFF, 0xF5A78CFF, 0xF69B7EFF, 0xFF8A65FF, 0xFF7A50FF, 0xFB683AFF, 0xF95523FF, 0xF34413FF, 0xD8390AFF, 0xB22F07FF, 0x8F2905FF, 0x742406FF, 0x57200AFF, 0x2b0d05FF,
            0xFCEBCFFF, 0xFCDAB2FF, 0xFCCB96FF, 0xFBBE7CFF, 0xF3B067FF, 0xFCA45FFF, 0xFB913EFF, 0xFF7F1CFF, 0xE76E09FF, 0xD06B00FF, 0xB05F00FF, 0x9C5300FF, 0x824A00FF, 0x6E3B06FF, 0x5A2E05FF, 0x351700FF,
            0xFFF6C9FF, 0xFFEDB1FF, 0xFFE593FF, 0xFDDC74FF, 0xF9D36AFF, 0xFFC94FFF, 0xFFBD1EFF, 0xF4B600FF, 0xD2A200FF, 0xB78E04FF, 0x9D7D00FF, 0x8C6F07FF, 0x776300FF, 0x614F00FF, 0x503D00FF, 0x392900FF,
            0xFFFFCDFF, 0xFEFDBCFF, 0xFCF9A3FF, 0xFBF583FF, 0xFAF065FF, 0xFFEA2BFF, 0xFEEB04FF, 0xE9DC00FF, 0xD7CC00FF, 0xC4BA00FF, 0xADA500FF, 0x9A9200FF, 0x7C7700FF, 0x605B00FF, 0x4F4C00FF, 0x343700FF,
            0xE4FFBDFF, 0xDBFDBCFF, 0xCEF9A3FF, 0xC4F48EFF, 0xBAEE76FF, 0xB1E760FF, 0x9FEF29FF, 0x97E313FF, 0x7FC900FF, 0x78BB00FF, 0x67A201FF, 0x5C8E02FF, 0x527F01FF, 0x426600FF, 0x334F00FF, 0x203700FF,
            0xE1F8D8FF, 0xC7F8C2FF, 0xADF5ABFF, 0x9EEC97FF, 0x85E47FFF, 0x66DF66FF, 0x4EDB5FFF, 0x23D138FF, 0x12C72CFF, 0x0EB522FF, 0x109B17FF, 0x0A8913FF, 0x077311FF, 0x066011FF, 0x004F06FF, 0x003100FF,
            0xD5FFF8FF, 0xC5FDF4FF, 0xA8F7E5FF, 0x99F0D7FF, 0x7BF1C6FF, 0x60E6B1FF, 0x53E0A0FF, 0x1CE76FFF, 0x0DD763FF, 0x0AD056FF, 0x02B548FF, 0x04A94DFF, 0x009A40FF, 0x088133FF, 0x005A2BFF, 0x00331AFF,
            0xD9FDFEFF, 0xB5FBFEFF, 0xA8FFFFFF, 0x92FFFFFF, 0x7AFDFFFF, 0x66F9FDFF, 0x0FF8FEFF, 0x1CE7F5FF, 0x06C5C9FF, 0x0BA2B2FF, 0x04A39EFF, 0x079A8EFF, 0x008B7DFF, 0x027A7AFF, 0x004D4FFF, 0x003135FF,
            0xE2F5FFFF, 0xB4EBFFFF, 0xA3DAFFFF, 0x89CCF8FF, 0x79D1FFFF, 0x61C3F7FF, 0x59C5F3FF, 0x3FB9F6FF, 0x05A4FAFF, 0x0592F0FF, 0x0677B8FF, 0x00649AFF, 0x065A77FF, 0x004974FF, 0x003154FF, 0x00243eFF,
            0xD1DDFFFF, 0xB5D0EFFF, 0xA7BFF6FF, 0x90B6F4FF, 0x85AAF7FF, 0x67AAFFFF, 0x5386FFFF, 0x437AFFFF, 0x3A6AFFFF, 0x044EFCFF, 0x034EF7FF, 0x0A37BFFF, 0x09268BFF, 0x062871FF, 0x001D4CFF, 0x001435FF,
            0xC7C3FFFF, 0xC7C6FEFF, 0x9D9CFFFF, 0x9194F6FF, 0x7E81FEFF, 0x776FF7FF, 0x595AFFFF, 0x4852FFFF, 0x2D2EFFFF, 0x2020F8FF, 0x0400E1FF, 0x0000DDFF, 0x010097FF, 0x000086FF, 0x03005DFF, 0x020035FF,
            0xE1D4FFFF, 0xD8ACFFFF, 0xCD9BFFFF, 0xC88DFAFF, 0xBD8AF9FF, 0xB160FFFF, 0xAA52FEFF, 0x9841FDFF, 0x8726FFFF, 0x8700F5FF, 0x7200F4FF, 0x5C00B7FF, 0x460489FF, 0x350077FF, 0x28004FFF, 0x1c0037FF,
            0xFFC7FFFF, 0xFFB8FFFF, 0xFFAAFFFF, 0xF194F7FF, 0xF681F5FF, 0xF77CF1FF, 0xF76DFFFF, 0xF956FFFF, 0xF82FFFFF, 0xEA00E6FF, 0xD200CBFF, 0xB500B3FF, 0xA6009CFF, 0x8A0084FF, 0x6F005EFF, 0x330035FF,
            0xFDD2E6FF, 0xF9B5DAFF, 0xF7A4D4FF, 0xF198CBFF, 0xF682BDFF, 0xFF5FAEFF, 0xFF4CA9FF, 0xFF3CA4FF, 0xFF1A94FF, 0xF90979FF, 0xE80071FF, 0xC40061FF, 0x96004AFF, 0x670132FF, 0x4F0024FF, 0x310016FF,
};

// @todo Implement (extends the default_colors_256 -> both together form 1024 colors)
const uint32 default_colors_768[768] = {};

#endif