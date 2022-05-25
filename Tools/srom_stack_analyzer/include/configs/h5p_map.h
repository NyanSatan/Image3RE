#ifndef H5P_MAP_H
#define H5P_MAP_H

#include <platform.h>

static const function_t h5p_map[] = {
    {"start", 0x0},
    {"sub_340", 0x340},
    {"sub_348", 0x348},
    {"sub_350", 0x350},
    {"sub_358", 0x358},
    {"sub_360", 0x360},
    {"sub_368", 0x368},
    {"sub_374", 0x374},
    {"sub_37C", 0x37c},
    {"sub_3D4", 0x3d4},
    {"sub_3F0", 0x3f0},
    {"arm_read_cr", 0x418},
    {"arm_write_cr", 0x420},
    {"sub_42C", 0x42c},
    {"sub_434", 0x434},
    {"arm_write_dar", 0x440},
    {"arm_write_ttb", 0x44c},
    {"sub_458", 0x458},
    {"sub_460", 0x460},
    {"sub_468", 0x468},
    {"sub_470", 0x470},
    {"sub_478", 0x478},
    {"sub_480", 0x480},
    {"sub_48C", 0x48c},
    {"sub_494", 0x494},
    {"sub_49C", 0x49c},
    {"sub_4A4", 0x4a4},
    {"sub_4AC", 0x4ac},
    {"sub_4B4", 0x4b4},
    {"sub_4C0", 0x4c0},
    {"sub_584", 0x584},
    {"sub_648", 0x648},
    {"sub_650", 0x650},
    {"sub_65C", 0x65c},
    {"sub_664", 0x664},
    {"sub_670", 0x670},
    {"sub_678", 0x678},
    {"sub_684", 0x684},
    {"sub_68C", 0x68c},
    {"arm_flush_tlbs", 0x698},
    {"nullsub_5", 0x6ac},
    {"sub_6B0", 0x6b0},
    {"sub_6FC", 0x6fc},
    {"sub_750", 0x750},
    {"sub_75C", 0x75c},
    {"sub_76C", 0x76c},
    {"arch_enable_ints", 0x778},
    {"arch_disable_ints", 0x78c},
    {"sub_7A0", 0x7a0},
    {"sub_7BC", 0x7bc},
    {"sub_7CC", 0x7cc},
    {"sub_7DC", 0x7dc},
    {"sub_7E4", 0x7e4},
    {"sub_7EC", 0x7ec},
    {"arch_halt", 0x7fc},
    {"arch_spin", 0x808},
    {"swift_read_l2cerrsts", 0x80c},
    {"swift_write_l2cerrsts", 0x814},
    {"swift_read_l2cerradr", 0x820},
    {"swift_write_l2cerradr", 0x828},
    {"boot_image", 0x840},
    {"main", 0x884},
    {"sub_B00", 0xb00},
    {"unmask_int", 0xb28},
    {"sub_B54", 0xb54},
    {"timer_get_ticks", 0xb74},
    {"sub_BA4", 0xba4},
    {"wdt_chip_reset", 0xbc0},
    {"timer_get_entropy", 0xc14},
    {"sub_C24", 0xc24},
    {"install_int_handler", 0xc30},
    {"sub_C6C", 0xc6c},
    {"sub_C8C", 0xc8c},
    {"platform_fiq", 0xcbc},
    {"timer_deadline", 0xd18},
    {"timer_deadline_enter", 0xd48},
    {"interrupt_mask_all", 0xdc4},
    {"platform_irq", 0xdd8},
    {"timer_init", 0xe78},
    {"sub_EC0", 0xec0},
    {"sub_F40", 0xf40},
    {"sub_F80", 0xf80},
    {"dma_setup_channel", 0xfcc},
    {"sub_1108", 0x1108},
    {"aes_hw_crypto_cmd", 0x1128},
    {"sub_1380", 0x1380},
    {"sub_13B8", 0x13b8},
    {"sub_13E4", 0x13e4},
    {"sub_1474", 0x1474},
    {"sub_14C4", 0x14c4},
    {"sub_1510", 0x1510},
    {"h2fmi_clean_ecc", 0x1544},
    {"sub_155C", 0x155c},
    {"sub_15AC", 0x15ac},
    {"sub_15E8", 0x15e8},
    {"sub_164C", 0x164c},
    {"h2fmi_pio_read_sector", 0x1690},
    {"sub_16E8", 0x16e8},
    {"h2fmi_setup_bootpage_read", 0x1740},
    {"sub_187C", 0x187c},
    {"sub_1904", 0x1904},
    {"sub_1958", 0x1958},
    {"sub_19D0", 0x19d0},
    {"h2fmi_gate", 0x1a04},
    {"sub_1A34", 0x1a34},
    {"sub_1A64", 0x1a64},
    {"nand_read_bootblock", 0x1ac4},
    {"sha1_hw_calculate", 0x1c80},
    {"sub_1E00", 0x1e00},
    {"sub_1E38", 0x1e38},
    {"flash_spi_init", 0x1eb8},
    {"sub_1F6C", 0x1f6c},
    {"pke_do_exp", 0x2000},
    {"sub_2078", 0x2078},
    {"sub_20C8", 0x20c8},
    {"sub_2124", 0x2124},
    {"sub_2140", 0x2140},
    {"sub_217C", 0x217c},
    {"sub_21B8", 0x21b8},
    {"sub_2244", 0x2244},
    {"sub_236C", 0x236c},
    {"sub_2700", 0x2700},
    {"sub_2720", 0x2720},
    {"sub_2754", 0x2754},
    {"sub_2768", 0x2768},
    {"sub_27AC", 0x27ac},
    {"sub_27C4", 0x27c4},
    {"sub_27E8", 0x27e8},
    {"spi_init", 0x2814},
    {"spi_interrupt", 0x2888},
    {"sub_2A10", 0x2a10},
    {"sub_2B14", 0x2b14},
    {"sub_2C3C", 0x2c3c},
    {"sub_2D98", 0x2d98},
    {"sub_2DE4", 0x2de4},
    {"sub_2E60", 0x2e60},
    {"sub_2F80", 0x2f80},
    {"sub_2F88", 0x2f88},
    {"sub_2F90", 0x2f90},
    {"sub_2FA4", 0x2fa4},
    {"sub_2FB8", 0x2fb8},
    {"nullsub_1", 0x2fbc},
    {"sub_2FFC", 0x2ffc},
    {"sub_3054", 0x3054},
    {"sub_3078", 0x3078},
    {"sub_309C", 0x309c},
    {"sub_30F4", 0x30f4},
    {"sub_3144", 0x3144},
    {"sub_3274", 0x3274},
    {"sub_3370", 0x3370},
    {"sub_342C", 0x342c},
    {"sub_34DC", 0x34dc},
    {"sub_3548", 0x3548},
    {"sub_358C", 0x358c},
    {"sub_367C", 0x367c},
    {"synopsys_otg_start", 0x378c},
    {"sub_381C", 0x381c},
    {"synopsys_otg_int_handler", 0x387c},
    {"sub_3B58", 0x3b58},
    {"sub_3B98", 0x3b98},
    {"quantify_size", 0x3bc0},
    {"required_size", 0x3bc4},
    {"next_block", 0x3bd4},
    {"prev_block", 0x3bdc},
    {"compute_bin", 0x3bf4},
    {"free", 0x3c08},
    {"heap_add_chunk", 0x3d38},
    {"assign_chunk", 0x3ddc},
    {"malloc", 0x3e7c},
    {"memalign", 0x3efc},
    {"image3GetTagStruct", 0x40c0},
    {"image3GetTagUnsignedNumber", 0x41a0},
    {"image3GetNestedImage", 0x4250},
    {"image3InstantiateFromBuffer", 0x4268},
    {"image3ValidateSignature", 0x4370},
    {"image3Discard", 0x45f8},
    {"image3Find", 0x4640},
    {"image3Free", 0x4660},
    {"image3Malloc", 0x466c},
    {"image3PKIVerifyHash", 0x467c},
    {"image3SHA1Generate", 0x46a4},
    {"image3AESDecryptUsingLocalKey", 0x46b0},
    {"verifyNumberRelation", 0x474c},
    {"verifyIntersectingBits", 0x481c},
    {"image3_load", 0x48c4},
    {"image3_process_superblock", 0x4d84},
    {"platform_consume_nonce", 0x4f00},
    {"platform_start", 0x4fc0},
    {"sub_4FD8", 0x4fd8},
    {"sub_4FE0", 0x4fe0},
    {"platform_get_board_id", 0x4fec},
    {"platform_get_keybag_selector", 0x5020},
    {"sub_5050", 0x5050},
    {"sub_5064", 0x5064},
    {"sub_5078", 0x5078},
    {"platform_set_dfu_status", 0x5088},
    {"sub_5098", 0x5098},
    {"platform_get_ecid_image_personalization_required", 0x50c4},
    {"platform_get_nonce", 0x50d0},
    {"platform_enable_boot_interface", 0x512c},
    {"platform_init", 0x5260},
    {"platform_mmu_init", 0x526c},
    {"platform_quiesce_hardware", 0x52c4},
    {"sub_52D8", 0x52d8},
    {"sub_52E4", 0x52e4},
    {"sub_52F0", 0x52f0},
    {"platform_early_init", 0x52fc},
    {"platform_get_boot_device", 0x54e4},
    {"platform_bootprep", 0x562c},
    {"chipid_get_production_mode", 0x56c0},
    {"chipid_clear_production_mode", 0x56d0},
    {"sub_56E0", 0x56e0},
    {"sub_56F0", 0x56f0},
    {"sub_5700", 0x5700},
    {"chipid_get_ecid_image_personalization_required", 0x5710},
    {"sub_5720", 0x5720},
    {"sub_5730", 0x5730},
    {"sub_5738", 0x5738},
    {"sub_575C", 0x575c},
    {"chipid_set_fuse_lock", 0x5838},
    {"sub_584C", 0x584c},
    {"sub_5878", 0x5878},
    {"sub_58F4", 0x58f4},
    {"nullsub_2", 0x5940},
    {"sub_5944", 0x5944},
    {"sub_5948", 0x5948},
    {"sub_5980", 0x5980},
    {"sub_59B0", 0x59b0},
    {"sub_59C8", 0x59c8},
    {"clock_gate", 0x5a70},
    {"sub_5AB8", 0x5ab8},
    {"sub_5B5C", 0x5b5c},
    {"clocks_quiesce", 0x5c38},
    {"sub_5C64", 0x5c64},
    {"sub_5EC0", 0x5ec0},
    {"platform_watchdog_tickle", 0x5f2c},
    {"platform_reset", 0x5f38},
    {"sub_5F60", 0x5f60},
    {"prepare_and_jump", 0x5f80},
    {"sub_5FC0", 0x5fc0},
    {"sub_5FEC", 0x5fec},
    {"sub_6014", 0x6014},
    {"sub_6044", 0x6044},
    {"sub_60F8", 0x60f8},
    {"nullsub_3", 0x6150},
    {"debug_init", 0x6154},
    {"panic", 0x6190},
    {"stack_chk_fail", 0x6268},
    {"sys_init", 0x6280},
    {"_irq_exit_critical_section", 0x62c0},
    {"_irq_enter_critical_section", 0x62e4},
    {"exit_critical_section", 0x6310},
    {"enter_critical_section", 0x6340},
    {"sub_6380", 0x6380},
    {"sub_6398", 0x6398},
    {"sub_63C4", 0x63c4},
    {"sub_63E8", 0x63e8},
    {"task_create", 0x6400},
    {"task_init", 0x64bc},
    {"sub_6528", 0x6528},
    {"task_yield", 0x6538},
    {"task_sleep", 0x6624},
    {"sub_6664", 0x6664},
    {"sub_669C", 0x669c},
    {"idle_task", 0x66e0},
    {"sub_67E0", 0x67e0},
    {"sub_681C", 0x681c},
    {"sub_6868", 0x6868},
    {"sub_68AC", 0x68ac},
    {"sub_6900", 0x6900},
    {"sub_6910", 0x6910},
    {"time_has_elapsed", 0x6940},
    {"sub_6980", 0x6980},
    {"security_allow_modes", 0x699c},
    {"sub_69E0", 0x69e0},
    {"sub_6A28", 0x6a28},
    {"security_validate_image", 0x6a64},
    {"security_set_production_override", 0x6a80},
    {"security_get_production_override", 0x6a98},
    {"security_init", 0x6ad4},
    {"arm_clean_dcache_line", 0x6b80},
    {"sub_6B8C", 0x6b8c},
    {"sub_6B98", 0x6b98},
    {"sub_6BA4", 0x6ba4},
    {"sub_6BD8", 0x6bd8},
    {"sub_6C0C", 0x6c0c},
    {"sub_6C38", 0x6c38},
    {"sub_6C58", 0x6c58},
    {"sub_6C6C", 0x6c6c},
    {"sub_6C7C", 0x6c7c},
    {"arm_mmu_map_section_range", 0x6c94},
    {"arm_mmu_init", 0x6db4},
    {"arm_mmu_map_section", 0x6df4},
    {"arch_cpu_init_posttasks", 0x6e80},
    {"platform_get_boot_trampoline", 0x6e84},
    {"arch_cpu_quiesce", 0x6ecc},
    {"arch_cpu_init", 0x6f04},
    {"arch_task_context_switch", 0x6f40},
    {"arch_get_entropy", 0x6f80},
    {"arm_irq", 0x6fc0},
    {"arm_fiq", 0x7008},
    {"arm_undefined", 0x7050},
    {"arm_syscall", 0x7088},
    {"arm_prefetch_abort", 0x70c0},
    {"arm_data_abort", 0x70fc},
    {"arm_reserved", 0x7134},
    {"_arm_fiq", 0x715c},
    {"_arm_irq", 0x7170},
    {"sub_7184", 0x7184},
    {"sub_719C", 0x719c},
    {"sub_71B4", 0x71b4},
    {"sub_71E4", 0x71e4},
    {"sub_7240", 0x7240},
    {"sub_726C", 0x726c},
    {"trampoline", 0x72c0},
    {"aes_crypto_cmd", 0x7300},
    {"sub_7380", 0x7380},
    {"flash_nor_init", 0x73c0},
    {"flash_nor_register", 0x73d4},
    {"sha1_calculate", 0x7400},
    {"sub_7444", 0x7444},
    {"usb_quiesce", 0x7458},
    {"sub_7470", 0x7470},
    {"sub_74C0", 0x74c0},
    {"sub_74E8", 0x74e8},
    {"sub_74FC", 0x74fc},
    {"sub_755C", 0x755c},
    {"sub_7598", 0x7598},
    {"sub_75D8", 0x75d8},
    {"sub_75F8", 0x75f8},
    {"sub_7620", 0x7620},
    {"sub_767C", 0x767c},
    {"sub_7760", 0x7760},
    {"sub_7BFC", 0x7bfc},
    {"sub_7C10", 0x7c10},
    {"sub_7C54", 0x7c54},
    {"sub_7C7C", 0x7c7c},
    {"sub_7EB8", 0x7eb8},
    {"sub_7F80", 0x7f80},
    {"sub_7FB8", 0x7fb8},
    {"sub_7FEC", 0x7fec},
    {"sub_8094", 0x8094},
    {"sub_80BC", 0x80bc},
    {"handle_interface_request", 0x8160},
    {"getDFUImage", 0x8280},
    {"cbuf_create", 0x82e8},
    {"sub_8380", 0x8380},
    {"sub_83F4", 0x83f4},
    {"image_free", 0x8440},
    {"image_create_from_memory", 0x8458},
    {"image_find", 0x8480},
    {"image_load", 0x8498},
    {"find_boot_images", 0x8518},
    {"rsa_set_giant", 0x85c0},
    {"extension_octet_string_value", 0x8618},
    {"verify_pkcs1_sig", 0x8648},
    {"verify_signature_with_chain", 0x87bc},
    {"sub_8D50", 0x8d50},
    {"sub_8D60", 0x8d60},
    {"sub_8D68", 0x8d68},
    {"sub_8D70", 0x8d70},
    {"sub_8D94", 0x8d94},
    {"sub_8EF8", 0x8ef8},
    {"sub_8FA4", 0x8fa4},
    {"sub_9134", 0x9134},
    {"lookup_blockdev", 0x92a4},
    {"platform_cache_operation", 0x9300},
    {"platform_get_security_epoch", 0x93c0},
    {"sub_9400", 0x9400},
    {"sub_94BC", 0x94bc},
    {"sub_95C0", 0x95c0},
    {"sub_9600", 0x9600},
    {"platform_get_cpfm", 0x9640},
    {"sub_9680", 0x9680},
    {"sub_96C0", 0x96c0},
    {"sub_96F4", 0x96f4},
    {"__udivdi3", 0x976c},
    {"sub_983C", 0x983c},
    {"sub_9854", 0x9854},
    {"sub_988C", 0x988c},
    {"memset", 0x98c0},
    {"bzero", 0x98d8},
    {"sub_9A00", 0x9a00},
    {"sub_9A10", 0x9a10},
    {"swap32", 0x9a80},
    {"sub_9AC0", 0x9ac0},
    {"memmove", 0x9acc},
    {"calloc", 0x9e18},
    {"memcmp", 0x9e40},
    {"sub_9E80", 0x9e80},
    {"sub_9E94", 0x9e94},
    {"sub_9EB8", 0x9eb8},
    {"sub_9F84", 0x9f84},
    {"do_printf", 0x9fd4},
    {"vsnprintf", 0xa734},
    {"snprintf", 0xa794},
    {"sub_A7C0", 0xa7c0},
    {"puts", 0xa7f0},
    {"strncat", 0xa840},
    {"sub_A8C0", 0xa8c0},
    {"sub_A900", 0xa900},
    {"sub_A980", 0xa980},
    {"sub_A9C0", 0xa9c0},
    {"sub_AA00", 0xaa00},
    {"DERDecodeItem", 0xaa40},
    {"DERParseBitString", 0xaab8},
    {"DERDecodeSeqInit", 0xaafc},
    {"DERDecodeSeqContentInit", 0xab30},
    {"DERDecodeSeqNext", 0xab40},
    {"DERParseSequenceContent", 0xab74},
    {"DERParseSequence", 0xac58},
    {"DEREncodeSHA1DigestInfo", 0xaca4},
    {"DEROidCompare", 0xad40},
    {"platform_get_chip_id", 0xad80},
    {"platform_get_chip_revision", 0xadc0},
    {"platform_get_ecid_id", 0xae00},
    {"platform_get_chip_epoch", 0xae40},
    {"platform_get_production_mode", 0xae80},
    {"platform_get_security_domain", 0xaec0},
    {"platform_get_entropy", 0xaf00},
    {"nullsub_4", 0xaf40},
    {"sub_AF80", 0xaf80},
    {"platform_halt", 0xafc0},
    {"platform_get_secure_mode", 0xb000},
    {"sub_B040", 0xb040},
    {"sub_B080", 0xb080},
    {"sub_B0C0", 0xb0c0},
};

#endif