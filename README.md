# language-games
-- BUILDING --
1. Clone the repo
2. Aquire an SFML build (or build it yourself)
3. Copy all sfml-xxxx-d.lib AND sfml-xxxx.lib files to ../external/sfml/
4. If you are using Visual Studio 2022 you can run "../vsstudio/generative_vs2022.bat" as is and it will build the VS project and link everything for you
5. If you are using another version of Visual Studio, the bat file can be easily modified (it's one line)
6. If you are using something else, it hasn't been tested, but if you are familiar with CMake and comfortable with your IDE, you should be able to figure it out
7. Open the VS Project and run it once in Debug mode, and another time in Release mode. You will get errors.
8. Copy all DEBUG sfml .dll files into ../vsstudio/source/Debug
9. Copy all RELEASE sfml .dll files into ../vsstudio/source/Release
10. Errors should be gone now

-- LICENSE --
Font licesnsed under the SIL Open Font License, which is provided.

-- TODO --
Better read me.