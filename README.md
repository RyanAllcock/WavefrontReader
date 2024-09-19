# OBJ File Reader
Used to import .obj files and .mtl files as a model of objects and materials.

Currently implements basic wavefront file loading and data storage.

This document will ideally develop into an intuitive resource in implementing wavefront file reading and rendering software.

## How to use
- To compile, use the command *g++ -o fileReader.exe main.cpp* inside the project directory, using the MinGW compiler with the stb-image library file present.
- To run, use the command *.\fileReader.exe param_obj* with *param_obj* being the relative path of an .obj file.
- This program parses and displays the present data inside the referenced files.

## Data format
- File parameter must be filled with a reference to an .obj format file to produce model data.
- Referenced files must be present, with all internally-stated relative paths starting from the executable's directory.
- Referenced files must contain statements in the syntax given below for the correct data to be read.

## References
Image and map data is currently loaded using the stb-image Github header library. (https://github.com/nothings/stb)

## Wavefront file formats (https://en.wikipedia.org/wiki/Wavefront_.obj_file)
Developed by Wavefront Technologies for the Advanced Visualizer animation package. These formats are highly descriptive and versatile, featuring basic and free-form geometry for model data, as well as colours, maps and rendering options for material data. They have since been made usable by many 3D graphics applications, making them a reliable foundation for producing rendering software which loads models from files.

### Syntax
Used for reading the files. Data accepted, based on each specific command, can be floating point *Values*, integer *Indices*, string *Types*, string *Files*, or mixed *Identifiers*.

- *Values* represent raw vertex data.
- *Indices* reference previously-defined vertex data indices. Multiple indices are separated by '/' characters. Positive indices count up from 1 to the total number of currently-defined vertices of the relevant type, while negative indices count backwards from the last-defined vertex of the relevant type.
- *Types* represent command-specific, pre-defined phrases denoting behaviour of a command.
- *Files* represent local file path references to a file on the machine.
- *Identifiers* represent integers or pre-define phrases specific to the command.

Values following a statement's keyword are separated using whitespace along the statement's current line in file, unless a statement line break is given.

Some attributes have suggested ranges, given from the specifications of the source material used in this document; these possible range values could conflict with other software which load/render wavefront files differently.

Primitive statements for formatting and specifying sub-values are given in a table below.

| Included | Keyword | Command | Data | Description | Format |
| ... | ...| ... | ... | ... | ... |
| Primitives (For both .obj files and .mtl files) ||||||
| Yes | # | Comment | - | Statement keywords given that aren't specified below are ignored | - |
| Yes | / | Index separator | - | Separate indices specified for elements | - |
| No  | \backslash\backslash | Statement line break | - | Continue current statement on next line, ignoring further input on this line | - |

## Wavefront .obj files (https://paulbourke.net/dataformats/obj/)
Contains the meshes of a model, represented using vectors and elements.

| Included | Keyword | Command | Data | Description | Format |
| ... | ...| ... | ... | ... | ... |
| Vertex Data ||||||
| Yes | v      | Geometric vertex | x y z w | Vertex at position (x,y,z) with optional weight (w) with default value 1.0 | Values |
| Yes | vt     | Texture coordinate | u v | Texture at position (u,v) with optional component (v) with default value 0 | Values |
| Yes | n      | Normal vector | x y z | Normal vector with direction (x,y,z) | Float |
| No  | vp     | ? | ? | ? | ? |
| No  | cstype | ? | ? | ? | ? |
| No  | deg    | ? | ? | ? | ? |
| No  | bmat   | ? | ? | ? | ? |
| No  | step   | ? | ? | ? | ? |
| Elements ||||||
| No  | p     | Point | ? | ? | ? |
| No  | l     | Line | ? | ? | ? |
| Yes | f     | Face | i j k l | Triangle face across (i,j,k) or Quad face across (i,j,k,l) if optional point (l) is given | Indices: vertex data, texture coordinate, normal |
| No  | curv  | ? | ? | ? | ? |
| No  | curv2 | ? | ? | ? | ? |
| No  | surf  | ? | ? | ? | ? |
| Free-form Statements ||||||
| No  | parm | ? | ? | ? | ? |
| No  | trim | ? | ? | ? | ? |
| No  | hole | ? | ? | ? | ? |
| No  | scrv | ? | ? | ? | ? |
| No  | sp   | ? | ? | ? | ? |
| No  | end  | ? | ? | ? | ? |
| Connectivity ||||||
| No  | con | ? | ? | ? | ? |
| Grouping ||||||
| No  | g  | ? | ? | ? | ? |
| Yes | s  | Smoothing group | n | Assign proceeding elements to smoothing group (n) for values >0 *or* avoid smoothing with values 'off' or 0 | Identifiers |
| No  | mg | ? | ? | ? | ? |
| No  | o  | ? | ? | ? | ? |
| Rendering Attributes ||||||
| No  | bevel      | ? | ? | ? | ? |
| No  | c_interp   | ? | ? | ? | ? |
| No  | d_interp   | ? | ? | ? | ? |
| No  | lod        | ? | ? | ? | ? |
| Yes | usemtl     | Material assignment | name | Use previously-allocated material (name) for proceeding element rendering | Identifiers |
| Yes | mtllib     | Material library access | filename | Obtain materials from (filename) to use for rendering model, discard unused materials | Files |
| No  | shadow_obj | ? | ? | ? | ? |
| No  | trace_obj  | ? | ? | ? | ? |
| No  | ctech      | ? | ? | ? | ? |
| No  | stech      | ? | ? | ? | ? |

## Wavefront .mtl files (https://paulbourke.net/dataformats/mtl/)
Contains the materials for rendering a model, represented using RGB values, attributes/flags and maps.

| Included | Keyword | Command | Data | Description | Format |
| ... | ...| ... | ... | ... | ... |
| Colour & Illumination ||||||
| Yes | Ka    | Ambient colour | r g b | Assign RGB value (r,g,b) to current material | Floats |
| Yes | Kd    | Diffuse colour | r g b | Assign lighting modifier value (r,g,b) | Floats |
| Yes | Ks    | Specular colour | r g b | Assign specular shading value (r,g,b) | Floats |
| Yes | Ke    | Emissive colour | r g b | Assign emissive shading value (r,g,b) | Floats |
| No  | illum | Illumination model | ? | ? | ? |
| Yes | Ns    | Specular exponent | e | Specify focus/concentration of the specular highlight (e), with range 0 to 1000 | Floats |
| Yes | Ni    | Optical density | o | Specify index of refraction (o), with range 0.001 to 10 | Floats |
| Yes | d     | Opaqueness | f | Specify overall dissolution (f), from fully dissolved to fully opaque in range 0 to 1 | Floats |
| No  | Tr    | Transparency | f | Alternative to *d*, except fully dissolved to fully opaque is ranged using (f) values 1 to 0 instead | Floats |
| No  | Tf    | ? | ? | ? | ? |
| Texture & Reflection Maps ||||||
| No  | map_Ka   | Ambient map | f | Specify ambient texture map relative path (f) | Files |
| Yes | map_Kd   | Diffuse map | f | Specify diffuse texture map relative path (f) | Files |
| No  | map_Ks   | Specular map | ? | ? | Files |
| No  | map_Ns   | Specular highlight map | ? | ? | Files |
| No  | map_d    | ? | ? | ? | ? |
| No  | map_bump | Bump map | ? | ? | ? |
| No  | bump     | Bump map | ? | ? | ? |
| No  | disp     | Displacement map | ? | ? | ? |
| No  | decal    | ? | ? | ? | ? |
| No  | refl     | ? | ? | ? | ? |
| Naming ||||||
| Yes | newmtl | Material specification |  |  | Identifiers |

## Parts to add
- type/error checking
- texture map options
- full texture mapping
- reflection mapping
- free-form geometry and elements
- full grouping
- illumination options
- models with dynamic-complexity data and shaders
- rendering attributes