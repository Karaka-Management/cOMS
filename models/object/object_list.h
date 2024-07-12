#ifndef TOS_MODELS_OBJECT_LIST_H
#define TOS_MODELS_OBJECT_LIST_H

// (null)
#define OBJ_UNASSIGNED 0x00000000
#define OBJ_ALCHEMY 0x000000CC
#define OBJ_BUCKET 0x000000D8
#define OBJ_CART 0x000000C8
#define OBJ_CHEESE 0x000000DB
#define OBJ_COMPASS 0x000000E3
#define OBJ_DECANTER 0x000000DA
#define OBJ_FORK 0x000000D0
#define OBJ_FURNACE 0x000000CB
#define OBJ_GADGETS 0x000000CD
#define OBJ_GLASS 0x000000D4
#define OBJ_GLASSES 0x000000E4
#define OBJ_GYROSCOPE 0x000000E0
#define OBJ_ICE 0x00000097
#define OBJ_JUG 0x000000D5
#define OBJ_KNIFE 0x000000D2
#define OBJ_LECTERN 0x000000CE
#define OBJ_LOOM 0x000000C9
#define OBJ_MONOCLE 0x000000E5
#define OBJ_NOODLE 0x000000DF
#define OBJ_PITCHER 0x000000D6
#define OBJ_RUSSIAN_EGG 0x000000E6
#define OBJ_SEXTANT 0x000000E2
#define OBJ_SNOW 0x00000096
#define OBJ_SOUP_PLATE 0x000000D3
#define OBJ_SPINNING_MACHINE 0x000000CF
#define OBJ_SPOON 0x000000D1
#define OBJ_STEAK 0x000000DC
#define OBJ_TELESCOPE 0x000000E1
#define OBJ_TRAIN 0x000000CA
#define OBJ_TRAY 0x000000D7
#define OBJ_TURKEY 0x000000DD
#define OBJ_WINE_GLASS 0x000000D9

// Ore
#define OBJ_ORE 0x04000000
#define OBJ_ALUMINUM 0x04000006
#define OBJ_AMBER 0x04000041
#define OBJ_AMETHYST 0x04000040
#define OBJ_BENITOITE 0x0400003C
#define OBJ_BISMUTH 0x04000007
#define OBJ_BLACK_OPAL 0x0400003B
#define OBJ_CLAY 0x040000AD
#define OBJ_COAL 0x04000042
#define OBJ_COBALT 0x04000008
#define OBJ_COBBLESTONE 0x04000052
#define OBJ_COPPER 0x04000009
#define OBJ_DIAMOND 0x04000043
#define OBJ_EARTH 0x04000002
#define OBJ_EMERALD 0x04000044
#define OBJ_FLINTSTONE 0x04000095
#define OBJ_GOLD 0x0400000A
#define OBJ_GRANITE 0x0400004F
#define OBJ_IRON 0x0400000B
#define OBJ_JADEIT 0x0400003F
#define OBJ_LIMESTONE 0x0400004E
#define OBJ_MANGANESE 0x0400000C
#define OBJ_MARBLE 0x04000050
#define OBJ_METEORITE 0x04000047
#define OBJ_NICKEL 0x0400000D
#define OBJ_PHOSPHORUS 0x0400005F
#define OBJ_PLATINUM 0x0400000E
#define OBJ_QUARTZ 0x04000051
#define OBJ_RED_BERYL 0x0400003A
#define OBJ_RED_MEAT 0x04000062
#define OBJ_RUBY 0x04000049
#define OBJ_SALT 0x04000060
#define OBJ_SAND 0x04000001
#define OBJ_SANDSTONE 0x0400005C
#define OBJ_SAPPHIRE 0x04000048
#define OBJ_SILVER 0x0400000F
#define OBJ_STONE 0x04000003
#define OBJ_SULFUR 0x0400005E
#define OBJ_TAAFFEIT 0x0400003E
#define OBJ_TANZANITE 0x0400003D
#define OBJ_TIN 0x040000AE
#define OBJ_TITANIUM 0x04000010
#define OBJ_TUNGSTEN 0x04000011
#define OBJ_VOLCANIC_ROCK 0x040000AC
#define OBJ_WHITE_MEAT 0x04000061
#define OBJ_ZINC 0x040000AF

// Block
#define OBJ_BLOCK 0x05000000
#define OBJ_COLOR_BLOCK_1 0x050000EA
#define OBJ_COLOR_BLOCK_10 0x050000FA
#define OBJ_COLOR_BLOCK_11 0x050000FD
#define OBJ_COLOR_BLOCK_12 0x050000FE
#define OBJ_COLOR_BLOCK_13 0x050000FF
#define OBJ_COLOR_BLOCK_14 0x05000100
#define OBJ_COLOR_BLOCK_15 0x05000101
#define OBJ_COLOR_BLOCK_16 0x05000102
#define OBJ_COLOR_BLOCK_2 0x050000F2
#define OBJ_COLOR_BLOCK_3 0x050000F3
#define OBJ_COLOR_BLOCK_4 0x050000F4
#define OBJ_COLOR_BLOCK_5 0x050000F5
#define OBJ_COLOR_BLOCK_6 0x050000F6
#define OBJ_COLOR_BLOCK_7 0x050000F7
#define OBJ_COLOR_BLOCK_8 0x050000F8
#define OBJ_COLOR_BLOCK_9 0x050000F9
#define OBJ_EARTH_BLOCK 0x050000EB
#define OBJ_GLASS_BLOCK 0x050000ED
#define OBJ_GRANITE_BLOCK 0x05000104
#define OBJ_GRASS_BLOCK 0x050000F1
#define OBJ_GRAVEL_BLOCK 0x05000103
#define OBJ_ICE_BLOCK 0x050000EC
#define OBJ_ICE_COVER 0x050000EF
#define OBJ_LAVA_BLOCK 0x050000FC
#define OBJ_SAND_BLOCK 0x050000E7
#define OBJ_SNOW_BLOCK 0x050000EE
#define OBJ_SNOW_COVER 0x050000F0
#define OBJ_STONE_BLOCK 0x050000E8
#define OBJ_WATER_BLOCK 0x050000FB
#define OBJ_WOOD_BLOCK 0x050000E9

// Herb
#define OBJ_HERB 0x06000000
#define OBJ_ACONITUM 0x0600002D
#define OBJ_ALMONDS 0x0600006B
#define OBJ_ANTHURIUM 0x06000030
#define OBJ_APPLE 0x06000081
#define OBJ_AQUILEGIA 0x06000031
#define OBJ_ARNICA 0x06000024
#define OBJ_ARUM 0x06000032
#define OBJ_ASPARAGUS 0x06000084
#define OBJ_BANANA 0x06000086
#define OBJ_BARLEY 0x0600004C
#define OBJ_BASIL 0x0600008F
#define OBJ_BEAN 0x0600007B
#define OBJ_BETONY 0x0600001D
#define OBJ_BLACK_ANTHURIUM 0x060000A8
#define OBJ_BLACK_BACCARA 0x060000AB
#define OBJ_BLACK_DAHLIA 0x060000A9
#define OBJ_BLACK_MAGIC 0x060000A7
#define OBJ_BLACK_ORCHID 0x060000AA
#define OBJ_BLACKBERRY 0x06000063
#define OBJ_BLACKWATER 0x060000A4
#define OBJ_BLEEDING_HEART 0x06000039
#define OBJ_BLOOD_FLOWER 0x06000019
#define OBJ_BROCCOLI 0x06000079
#define OBJ_BRUGMANSIA 0x06000033
#define OBJ_BUCKWHEAT 0x0600004D
#define OBJ_CABBAGE 0x06000080
#define OBJ_CACAO 0x06000067
#define OBJ_CALADIUM 0x06000034
#define OBJ_CAMELLIA 0x06000094
#define OBJ_CARROT 0x0600007C
#define OBJ_CASHEW 0x0600006C
#define OBJ_CAULIFLOWER 0x0600007A
#define OBJ_CHERRY 0x06000082
#define OBJ_CHESTNUTS 0x0600006A
#define OBJ_CINAMON 0x0600008D
#define OBJ_COCONUT 0x0600006D
#define OBJ_COFFEE 0x06000068
#define OBJ_CONEFLOWER 0x06000021
#define OBJ_CONSOLIDA 0x06000035
#define OBJ_CORN 0x0600004A
#define OBJ_CORPSE_FLOWER 0x0600009B
#define OBJ_COTTON 0x06000028
#define OBJ_CUCUMBER 0x06000075
#define OBJ_DELPHINIUM 0x06000036
#define OBJ_DESERT_ROSE 0x0600002F
#define OBJ_DRAGONFRUIT 0x0600008A
#define OBJ_FIBRE 0x06000046
#define OBJ_FLAME_LILY 0x06000037
#define OBJ_FOXGLOVE 0x0600001C
#define OBJ_GARLIC 0x0600007F
#define OBJ_GHOST_ORCHID 0x0600009D
#define OBJ_GINGER 0x0600008C
#define OBJ_GINGKO 0x06000016
#define OBJ_GRAPES 0x0600008B
#define OBJ_GREEN_ONION 0x06000078
#define OBJ_HAZELNUT 0x0600006E
#define OBJ_HENBANE 0x06000020
#define OBJ_HOLLYHOCK 0x060000A2
#define OBJ_HYACINTH 0x060000A3
#define OBJ_HYDRANGEA 0x060000A0
#define OBJ_IRIS 0x06000038
#define OBJ_JASMINE 0x06000091
#define OBJ_KALE 0x06000085
#define OBJ_LATHYRUS 0x0600002A
#define OBJ_LAVENDER 0x06000014
#define OBJ_LEMON 0x06000098
#define OBJ_LETTUCE 0x06000071
#define OBJ_LILY 0x0600001E
#define OBJ_LINEN 0x06000029
#define OBJ_MACADAMIA 0x0600006F
#define OBJ_MARIGOLD 0x0600001B
#define OBJ_MARSH_MALLOW 0x06000023
#define OBJ_MATCHA 0x06000099
#define OBJ_MIDDLEMIST 0x0600009C
#define OBJ_MIDNIGHT_MYSTIC 0x060000A6
#define OBJ_MIDNIGHT_RUFFLES 0x060000A5
#define OBJ_MUSHROOM 0x06000072
#define OBJ_NIGHTSHADE 0x06000022
#define OBJ_ONION 0x06000077
#define OBJ_ORANGE 0x06000083
#define OBJ_OREGANO 0x0600008E
#define OBJ_PEACH 0x06000088
#define OBJ_PEANUTS 0x06000069
#define OBJ_PEAS 0x06000064
#define OBJ_PECANS 0x06000070
#define OBJ_PEONY 0x0600009E
#define OBJ_PEPERMINT 0x06000092
#define OBJ_PEPPER 0x06000073
#define OBJ_PERIWINKLE 0x0600001A
#define OBJ_PINEAPPLE 0x06000087
#define OBJ_PLATYCODON 0x060000A1
#define OBJ_POTATOS 0x06000065
#define OBJ_PUMPKIN 0x06000076
#define OBJ_RADISH 0x0600007E
#define OBJ_RASPBERRY 0x06000093
#define OBJ_RICE 0x0600004B
#define OBJ_ROSARY_BEAD 0x0600002C
#define OBJ_SAGE 0x06000017
#define OBJ_SENNA 0x0600001F
#define OBJ_SILK 0x06000027
#define OBJ_SOLANUM 0x0600002B
#define OBJ_SPINACH 0x0600007D
#define OBJ_STAR_ANISE 0x06000090
#define OBJ_TOMATO 0x06000074
#define OBJ_TURMERIC 0x06000018
#define OBJ_WALNUT 0x06000066
#define OBJ_WATERMELON 0x06000089
#define OBJ_WELWITSCHIA 0x0600009A
#define OBJ_WHEAT 0x06000045
#define OBJ_WHITE_BANEBERRY 0x0600002E
#define OBJ_WISTERIA 0x0600009F
#define OBJ_YARROW 0x06000025

// Liquid
#define OBJ_LIQUID 0x07000000
#define OBJ_LAVA 0x0700005D
#define OBJ_WATER 0x07000005

// Plant
#define OBJ_PLANT 0x0A000000
#define OBJ_ACACIA_WOOD 0x0A00005B
#define OBJ_ASH_WOOD 0x0A000058
#define OBJ_BAMBOO_WOOD 0x0A000054
#define OBJ_ELM_WOOD 0x0A000057
#define OBJ_MAHOGANY_WOOD 0x0A000059
#define OBJ_OAK_WOOD 0x0A000055
#define OBJ_PINE_WOOD 0x0A000056
#define OBJ_TEAK_WOOD 0x0A00005A
#define OBJ_WALNUT_WOOD 0x0A000053
#define OBJ_WOOD 0x0A000004

// Static Object
#define OBJ_STATIC_OBJECT 0x0B000000
#define OBJ_BAR 0x0B0000BA
#define OBJ_BARREL 0x0B0000BE
#define OBJ_BED 0x0B0000BB
#define OBJ_BOOK 0x0B0000C3
#define OBJ_CHAIR 0x0B0000B6
#define OBJ_CRATE 0x0B0000BF
#define OBJ_DOOR 0x0B0000C4
#define OBJ_FIREPLACE 0x0B0000C1
#define OBJ_IMAGE 0x0B0000B8
#define OBJ_LAMP 0x0B0000B5
#define OBJ_NIGHT_STAND 0x0B0000BC
#define OBJ_OVEN 0x0B0000BD
#define OBJ_PLATE 0x0B0000C0
#define OBJ_POT 0x0B0000C2
#define OBJ_SHELF 0x0B0000B9
#define OBJ_TABLE 0x0B0000B4
#define OBJ_TOOLS 0x0B0000C7
#define OBJ_WALL_ORNAMENTS 0x0B0000C6
#define OBJ_WINDOW 0x0B0000C5

// Static Texture
#define OBJ_STATIC_TEXTURE 0x0D000000
#define OBJ_CARPET 0x0D0000B7

#endif