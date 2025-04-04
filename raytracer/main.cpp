
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono> // Süre ölçümü için
#include "xmlparser.hpp"
#include "render.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Kullaniim: " << argv[0] << " <scene.xml>\n";
        return 1;
    }

    std::string inputPath = argv[1];
    Scene scene = XMLParser::parseFromFile(inputPath);

    std::string outputPath = "output/render.ppm";
    if (const char *envOutput = std::getenv("OUTPUT_NAME"))
    {
        outputPath = envOutput;
    }

    // Zaman ölçümü başlıyor
    auto start = std::chrono::high_resolution_clock::now();

    render(scene, outputPath);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    // Süreyi yazdır
    std::cout << "Render süresi: " << elapsed.count() << " saniye\n";
    std::cout << "Render tamamlandi: " << outputPath << "\n";

    return 0;
}