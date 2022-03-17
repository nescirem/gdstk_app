#include <filesystem>
#include <iostream>
#include <getopt.h>
#include <gdstk.h>

extern int optind,opterr,optopt;
extern char *optarg;

using namespace gdstk;

#define RST "\033[0m"
#define HLT "\033[1m"
#define RED "\033[31m"
#define DRED "\033[41m"
#define YEL "\033[33m"
#define CYA "\033[36m"

static struct option long_options[] = 
{
    {"input",required_argument,NULL,'i'},
    {"in-format",required_argument,NULL,'b'},
    {"out-format",required_argument,NULL,'a'},
    {"output",required_argument,NULL,'o'},
    {"tolerance",required_argument,NULL,'t'}
};

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

int main(int argc, char* argv[]) {
    namespace fs = std::filesystem;
    namespace cr = std::chrono;
    std::cout.precision(15);
    
    double tolerance = 1e-2;
    
    fs::path inFile, outFile;
    int inType = -1, outType = -1; //! 0-gds; 1-oas; -1-unknow
    
    int index = 0;
    int c = 0;
    while( EOF != (c = getopt_long(argc,argv,"i:o:t:b:a:",long_options,&index)) )
    {
        switch(c)
        {
            case 'i':
                inFile = optarg;
                break;
            case 'o':
                outFile = optarg;
                break;
            case 't':
                tolerance = atof(optarg);
                break;
            case 'b':
                if ( optarg == "gds" ){
                    inType = 0;
                }
                else if (optarg == "oas"){
                    inType = 1;
                }
                break;
            case 'a':
                if ( optarg == "gds" ){
                    outType = 0;
                }
                else if (optarg == "oas"){
                    outType = 1;
                }
                break;
            case '?':
                std::cout << "unknow option: " << optopt << std::endl;
                break;
            default:
                break;
        }   
    }
    
    if ( !fs::exists(inFile) || !fs::is_regular_file(inFile) ){
        std::cout << "can't find input file: " << inFile.string() << std::endl;
        return -1;
    }
    if ( !outFile.has_filename() ){
        std::cout << "invalid output filename: " << outFile.string() << std::endl;
        return -1;
    }
    
    if ( inType<0 ){
        inType = inFile.extension() == ".oas" ? 1 : 0;
    }
    if ( outType<0 ){
        outType = outFile.extension() == ".oas" ? 1 : 0;
    }
    
    std::cout << HLT <<"Tolerance: " << tolerance << RST << std::endl;
    
    Library lib;
    ErrorCode error_code = ErrorCode::NoError;
    std::cout << "Reading file: " << inFile.string() << std::endl;
    auto start = cr::high_resolution_clock::now();
    if ( inType == 0 ){
        lib = read_gds(inFile.c_str(), 0, tolerance, NULL, &error_code);
    }
    else if ( inType == 1 ){
        lib = read_oas(inFile.c_str(), 0, tolerance, &error_code);
    }
    if (error_code != ErrorCode::NoError){
        std::cout << DRED << "Error, code: " << as_integer(error_code) << RST << std::endl;
    }
    auto end = cr::high_resolution_clock::now();
    long long duration_us = cr::duration_cast<cr::microseconds>(end - start).count();
    long long duration_ms = cr::duration_cast<cr::milliseconds>(end - start).count();
    long long duration_s = cr::duration_cast<cr::seconds>(end - start).count();
    std::cout << "Done: "
        << duration_us << CYA << "[us] " << RST
        << duration_ms << YEL << "[ms] " << RST
        << duration_s  << RED << "[s]"  << RST << std::endl;
        
    std::cout << "Writing file: " << outFile.string() << std::endl;
    start = cr::high_resolution_clock::now();
    if ( outType == 0 ){
        error_code = lib.write_gds(outFile.c_str(), 0, NULL);
    }
    else if ( outType == 1 ){
        error_code = lib.write_oas(outFile.c_str(), 0, 6, OASIS_CONFIG_DETECT_ALL);
    }
    
    if (error_code != ErrorCode::NoError){
        std::cout << DRED << "Error, code: " << as_integer(error_code) << RST << std::endl;
    }
    end = cr::high_resolution_clock::now();
    duration_us = cr::duration_cast<cr::microseconds>(end - start).count();
    duration_ms = cr::duration_cast<cr::milliseconds>(end - start).count();
    duration_s = cr::duration_cast<cr::seconds>(end - start).count();
    std::cout << "Done: "
        << duration_us << CYA << "[us] " << RST
        << duration_ms << YEL << "[ms] " << RST
        << duration_s  << RED << "[s]"  << RST << std::endl;
    std::cout << std::endl;
    lib.clear();
    
    return 0;
}
