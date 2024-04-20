add_requires("cryptopp")
if is_mode("debug") then
    set_symbols("debug")
end
target("libun-chat5")
    set_kind("shared")
    add_files("lib/*.cpp")
    add_includedirs("include")
    add_packages("cryptopp")

target("test")
    set_kind("binary")
    add_deps("libun-chat5")
    add_files("test/*.cpp")
    add_includedirs("include")
