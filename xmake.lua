add_requires("jsoncpp")
set_languages("c99", "c++23")
if is_mode("debug") then
    set_symbols("debug")
end
target("libun-chat5")
    set_kind("shared")
    add_files("lib/*.cpp")
    add_includedirs("include")
    add_packages("jsoncpp")

target("test")
    set_kind("binary")
    add_files("test/*.cpp")
    add_includedirs("include")
    add_packages("jsoncpp")
