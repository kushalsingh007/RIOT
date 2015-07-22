unsigned char test_elf[] = {
  0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x54, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00,
  0x02, 0x02, 0x00, 0x05, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28, 0x00,
  0x05, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
  0x66, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
  0x01, 0x23, 0x82, 0xb0, 0x01, 0x93, 0x01, 0x9b, 0x00, 0x2b, 0xfc, 0xd1,
  0x01, 0x20, 0x02, 0xb0, 0x70, 0x47, 0x47, 0x43, 0x43, 0x3a, 0x20, 0x28,
  0x41, 0x72, 0x63, 0x68, 0x20, 0x52, 0x65, 0x70, 0x6f, 0x73, 0x69, 0x74,
  0x6f, 0x72, 0x79, 0x29, 0x20, 0x35, 0x2e, 0x31, 0x2e, 0x30, 0x00, 0x41,
  0x31, 0x00, 0x00, 0x00, 0x61, 0x65, 0x61, 0x62, 0x69, 0x00, 0x01, 0x27,
  0x00, 0x00, 0x00, 0x05, 0x43, 0x6f, 0x72, 0x74, 0x65, 0x78, 0x2d, 0x4d,
  0x30, 0x2b, 0x00, 0x06, 0x0c, 0x07, 0x4d, 0x09, 0x01, 0x12, 0x04, 0x14,
  0x01, 0x15, 0x01, 0x17, 0x03, 0x18, 0x01, 0x19, 0x01, 0x1a, 0x01, 0x1e,
  0x04, 0x00, 0x2e, 0x73, 0x68, 0x73, 0x74, 0x72, 0x74, 0x61, 0x62, 0x00,
  0x2e, 0x74, 0x65, 0x78, 0x74, 0x2e, 0x74, 0x65, 0x73, 0x74, 0x00, 0x2e,
  0x63, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x2e, 0x41, 0x52, 0x4d,
  0x2e, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x73, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00,
  0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x66, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x1f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb5, 0x00, 0x00, 0x00,
  0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned int test_elf_len = 428;
