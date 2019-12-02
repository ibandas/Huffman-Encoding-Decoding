cmake_minimum_required(VERSION 3.3)
project(huffman CXX)
include(.ipd/cmake/CMakeLists.txt)

set(COMMON src/bit_io.cxx)
set(SEVEN_COMMON ${COMMON} src/7coder.cxx)

# Add files for building huff, puff, and huffman_test to this list and
# they will be included in all three:
set(HUFF_COMMON
        ${COMMON})

# Example encoder/decoder for packed ASCII:
add_cxx_program(encode src/encode.cxx ${SEVEN_COMMON})
add_cxx_program(decode src/decode.cxx ${SEVEN_COMMON})
add_cxx_program(huffman src/huff_tree.cxx)

# Your Huffman encoder/decoder:
add_cxx_program(huff src/huff.cxx ${HUFF_COMMON})
add_cxx_program(puff src/puff.cxx ${HUFF_COMMON})

# Tests programs:
add_cxx_test(bitio_test src/bit_io.cxx test/bitio_test.cxx)
add_cxx_test(7coder_test test/7coder_test.cxx ${SEVEN_COMMON})
add_cxx_test(huffman_test test/huffman_test.cxx ${HUFF_COMMON})

