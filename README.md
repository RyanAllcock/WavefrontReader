# Wavefront File Reader
Used to import .obj files and .mtl files into object and material data structures.

Currently loads and stores basic, polyhedral geometry and coloured, textured materials of the wavefront file formats.

The file reader abstracts file access into buffer inspection operations, while the format readers use builders to construct a data context from the files' contents, ready to be rendered by a model viewer.

## How to use
- To compile, use the command *make* inside the project directory, using the MinGW compiler.
- To run the demo, use the command *.\main.exe* inside the project directory with *box.obj* and *box.mtl* being present.
- To run the demo with custom files, use the command *.\main.exe param_obj param_mtl* with *param_obj* and *param_mtl* being the relative path of an .obj and .mtl file, respectively.
- This program reads and displays the present data inside the referenced files.

## Wavefront file formats (https://en.wikipedia.org/wiki/Wavefront_.obj_file)
Developed by Wavefront Technologies for the Advanced Visualizer animation package. These formats are highly descriptive and versatile, featuring basic and free-form geometry for model data, as well as colours, maps and rendering options for material data. They have since been made usable by many 3D graphics applications, making them a reliable foundation for producing rendering software which loads models from files.

### Structure (https://paulbourke.net/dataformats/obj/)
An object represented inside an *.obj* file are given as vertex data, groups of elements and assigned materials. Materials represented inside an *.mtl* file are given as colours and textures.

Groups are used to provide different attributes to the same object, as well as for organisation.

Statements are used to describe data of a specific type. Stateful data are shared across file contents as they are read, including vertex data and libraries. Group and material specific data are assigned to the currently-selected groups or material, including elements and attributes.

### Overview
Currently-supported data is sufficient for a 3D, faceted, geometric polyhedron viewer. Vertex data is thus restricted in statement flexibility; no geometric vertex w value for free-form weights, and no optional entries for different dimensions of texture coordinates, and mandatory normal vectors to avoid needing to perform normal vector generation.

Here are the currently-supported statements of each file type (unsupported statements are summarised below the supported statements):

.obj file (supported):
| Keyword | Description | Data | Values |
| v | Geometric vertex | x y z | Position [x,y,z] in 3D space |
| vt | Texture coordinate | u v | Position [u,v] on 2D texture |
| vn | Normal vector | x y z | Normalised vector [x,y,z] in 3D space |
| g | Grouping | [names] | Groups [names] assigned proceeding elements |
| o | Object name | name | Object name given as general information |
| usemtl | Material | name | Material name identifies usage in proceeding elements until overwritten |
| mtllib | Material library | name | Library name provides access to materials used in file |

.obj file (unsupported)
| Keyword | Description |
| p | Point element |
| l | Line element |
| s | Smoothing |
| bevel | Bevel |
| c_interp | Colour interpolation |
| d_interp | Diffuse interpolation |
| lod | Level of detail |
| shadow_obj | Shadow corresponding object file reference |
| trace_obj | Ray tracing corresponding object file reference |
| ctech | Curve approximation technique |
| stech | Surface approximation technique |
| mg | Merge groups |
| vp | Free-form parameter vertex |
| cstype | Free-form type |
| deg | Free-form data degree |
| bmat | Free-form type basis matrix |
| step | Free-form spline step |
| curv | Free-form curve elements |
| curv2 | Free-form interlocked curve frame elements |
| surf | Free-form surface |
| parm | Free-form global parameters or knot vector |
| trim | Free-form surface trimming |
| hole | Free-form surface hole |
| sp | Free-form special point |
| con | Free-form instance connectivity |
| end | Free-form end of statements |
| scrc | ? |

.mtl file (supported):
| Keyword | Description | Data | Value |
| Ka | Ambient colour | r g b | Colour [r,g,b] values 0-1 |
| Kd | Diffuse colour | r g b | Colour [r,g,b] values 0-1 |
| Ks | Specular colour | r g b | Colour [r,g,b] values 0-1 |
| Ke | Emissive colour | r g b | Colour [r,g,b] values 0-1 |
| illum | Illumination model | i | Type i integer value |
| Ns | Specular exponent | f | Specular focus/concentration 0-1000 |
| Ni | Optical density | f | Index of refraction 0.001-10 |
| d | Opaqueness | f | Overall dissolution to opacity 0-1 |
| Tr | Transparency | f | Opaqueness reverse alternative 0-1 |
| map_Ka | Ambient map | name | File name reference |
| map_Kd | Diffuse map | name | File name reference |
| map_Ks | Specular map | name | File name reference |
| map_Ns | Specular highlight map | name | File name reference |
| map_d | ? | name | File name reference |
| map_bump | Bump map | name | File name reference |
| bump | Bump map alternative | name | File name reference |
| map_disp | Displacement map | name | File name reference |
| disp | Displacement map alternative | name | File name reference |
| decal | ? | name | File name reference |
| refl | ? | name | File name reference |
| newmtl | Material | name | Material name assigned proceeding properties |
