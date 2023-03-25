# ogg_ext
## Description
Extract the plain connection-type-ogg-vorbis to several ogg files. Previous offset disturbances are automatically avoided.  
This is made for my use and may be updated irregularly, but at any point it is provided free of charge and without warranty.  

## Dependences
OS：Windows  
Build with Visual Studio 2022 (v143).

## Usage
Just drop the connection-type-ogg-vorbis file to the exe.  
If you use cmd, please type `ogg_ext.exe C:\folder\connected-file-name.***`.

## Supported language for UI
English, 日本語, 普通话(简体字)  
The language is automatically taken from your environment and changed.
## Suppported file type
Can start with any characters.  
This program will divide file with
```
0x4f, 0x67, 0x67, 0x53, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x4f, 0x67, 0x67, 0x53
```
(OggS...OggS)  
The 0x00's can be any letter representing the information in the audio file.  

## License
Only main.cpp contains new creations; main.cpp is provided under [CC0](https://github.com/minfaox3/ogg_ext/blob/main/LICENSE).  
Use of the various libraries required to compile this file is subject to the licence indicated by the distributor of the library.  

## Notice
Any country, region, organization, or individual must comply with the laws.
