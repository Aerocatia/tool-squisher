 # Tool Squisher

 ## Shader Squishing

 Truncates shader types into 8-bit values. This is used to fix maps compiled
 with the leaked 1.10 beta build, enabling them to work with the official
 Halo Custom Edition release.

 The reason these maps break on official releases is because the leaked 1.10
 build reads and writes the shader type enum as an 8-bit value, with the
 other 8 bits now padded with a 0xFF byte when stored.

 Official releases read these enum fields as 16-bit, thus they will read a
 shader value like 0x0005 (shader_transparent_generic) as 0xFF05, leading to
 all sorts of fun undefined behavior, like non-shader_environment tags being
 read as shader_environment, which leads to a crash.

 Note that the shader struct exists in more tags than just shader tags, but
 only shader tags are impacted by this.

 ## Scenario Squishing

 Fixes variant numbers in conversations being incorrectly set to 0 when they
 are unused. This was due to a bug in the leaked 1.10 beta build's tool.exe
 where it didn't initialize the variant numbers to 0xFFFF. This breaks even
 on the leaked 1.10 build (thus conversations will ALWAYS be broken on the tag
 build which doesn't use cache files that could be fixed with this tool).

 ## Lens Flare Squishing

 Fixes Halo Editing Kit's tool.exe setting rotation scaling to 360 radians,
 which is likely higher (~58 times) than what the developer intended, 360
 degrees.
