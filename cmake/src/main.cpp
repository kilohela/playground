#include <fmt/core.h>
#include <fmt/format.h>
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

int main(int argc, char** argv){
    CLI::App app("a sample of CLI11 usage");

    std::vector<std::string> anonymous;
    std::string file, short_name, long_name;
    int integer_arg;

    app.add_option("anonymous", anonymous, "anonymous arguments (arguments with no name)");
    app.add_option("-f,--file", file, "file (arguments with both long and short name)");
    app.add_option("-s", short_name, "short name arguments");
    app.add_option("--long-name", long_name, "long name arguments");
    app.add_option("-i,--integer", integer_arg, "an integer arg, you must assign it") -> required();

    bool flag;
    app.add_flag("--flag", flag, "a bool flag") -> default_val(false);

    spdlog::info("argument parser initialization completed!");

    CLI11_PARSE(app, argc, argv)

    spdlog::info("argument parsed successfully!");

    if(flag){
        fmt::print("arguments:\n");
        fmt::print("    -f,--file:    {:10}\n", file);
        fmt::print("    -s:           {:10}\n", file);
        fmt::print("    --long-name:  {:10}\n", file);
        fmt::print("    -i,--integer: {:10}\n", integer_arg);
    }
    else fmt::print("Please specify --flag!");
    
    spdlog::info("Program end");
}