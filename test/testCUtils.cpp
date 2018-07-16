#include <iostream>
#include <chrono>
#include <vector>
#include <cstdint>
#include <cstring>

#define RLE_IMPLEMENTATION
#include "../Cutils/rle.hpp"

//Test data//
static const std::uint8_t str01[] = "Heeeeeeeeeeeeeeeeeeello worlllllllllllllllllllllllllllllllllllllllllld!";
static const std::uint8_t str02[] = "Heeeeeeeeeeeeeeeeeeello worlllllllllllllllllllllllllllllllllllllllllld!";
static const std::uint8_t str0[] = "Hello";

// ========================================================
// Run Length Encoding (RLE) tests:
// ========================================================

static void Test_RLE_EncodeDecode(const std::uint8_t * sampleData, const int sampleSize)
{
    
    std::vector<std::uint8_t> compressedBuffer(sampleSize * 4, 0); // RLE might make things bigger.
    std::vector<std::uint8_t> uncompressedBuffer(sampleSize, 0);

    // Compress:
    const int compressedSize = rle::easyEncode(sampleData, sampleSize,
                                               compressedBuffer.data(),
                                               compressedBuffer.size());

    std::cout << "RLE compressed size bytes   = " << compressedSize << "\n";
    std::cout << "RLE uncompressed size bytes = " << sampleSize << "\n";

    // Restore:
    const int uncompressedSize = rle::easyDecode(compressedBuffer.data(), compressedSize,
                                                 uncompressedBuffer.data(), uncompressedBuffer.size());

    // Validate:
    bool successful = true;
    if (uncompressedSize != sampleSize)
    {
        std::cerr << "RLE COMPRESSION ERROR! Size mismatch!\n";
        successful = false;
    }
    if (std::memcmp(uncompressedBuffer.data(), sampleData, sampleSize) != 0)
    {
        std::cerr << "RLE COMPRESSION ERROR! Data corrupted!\n";
        successful = false;
    }

    if (successful)
    {
        std::cout << "RLE compression successful!\n";
    }
    // No additional memory is allocated by the RLE encoder/decoder.
    // You have to provide big buffers.
    
}

static void Test_RLE()
{
    //std::cout << "> Testing random512...\n";
    //Test_RLE_EncodeDecode(random512, sizeof(random512));

    std::cout << "> Testing strings...\n";
    Test_RLE_EncodeDecode(str0, sizeof(str0));
    //Test_RLE_EncodeDecode(str1, sizeof(str1));
    //Test_RLE_EncodeDecode(str2, sizeof(str2));
    //Test_RLE_EncodeDecode(str3, sizeof(str3));

    //std::cout << "> Testing lenna.tga...\n";
    //Test_RLE_EncodeDecode(lennaTgaData, sizeof(lennaTgaData));
}

// ========================================================
// main() -- Unit tests driver:
// ========================================================
#define TEST(func) \
    do {    \
        std::cout << ">>> Testing "<< #func << " encoding/decoding.\n";\
        const auto startTime = std::chrono::system_clock::now();\
        Test_##func();\
        const auto endTime = std::chrono::system_clock::now();\
        std::chrono::duration<double> elapsedSeconds = endTime - startTime;\
        std::cout << ">>> " << #func << " completed in " << elapsedSeconds.count() << " seconds.\n";\
        std::cout << std::endl;\
    } while(0)

// ========================================================

int main()
{
    std::cout << "\nSTD:: Running unit tests for the compression algorithms...\n\n";
    TEST(RLE);
   return 0;
}