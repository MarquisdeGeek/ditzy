# Changes

So, what changed between the automated C++/C version and final, CMOC, version?


## CPU pruning

* fetch8 to embed the memory_read8 (`uint8_t fetch8(uint16_t addr) { return memory_fetch8(addr); }`)
* gsRegisterA et al
* All 'Unknown instruction's removed
* All intructions not used in ZK 1K chess are removed
* Assign to tmp8 done natively (i.e. without emfnumber) when result was unused
* Removing unneeded interupts, e.g. bool interuptStates
* Lots of (uint16_t) casts because bytes do not become ints when implicitly used in int calculations
* alu_add_u16s8 was custom to have CMOC byte handler
* alu_rra8 cast result down to u8
* alu_sub_u8u8b casts to u16 to get carry bit
* alu_sub_u16u16b casts, and `wasCarry = v2+v3>v1` needed
* alu_add_u8u8c casts to u16

# Future ideas
* return cycles can be refactored with break - 300+ bytes saved - but are there any nested switches?


