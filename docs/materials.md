# Materials

Contains all the per-model info like textures, combiners etc

Header:
| Offset | Size | Type   | Description     |
|--------|------|--------|-----------------|
| 0x0    | 0x4  | char[] | Magic "QMTL"    |
| 0x4    | 0x4  | u32    | Version         |
| 0x8    | 0x4  | u32    | Mask of Entries |

## Data entries

Several different types, corresponding to mask

| Bit | Description               |
|-----|---------------------------|
| 0   | Textures                  |
| 1   | Light Material            |
| 2   | Light LUTs                |
| 3   | Combiner Directives       |
| 4   | Fog LUTs                  |
| 5   | Gas LUTs                  |
| 6   | Depth/Alpha/Stencil tests |
| 7   |                           |

### Textures

### Light Material

### Light LUTs

| Offset | Size | Type   | Description             |
|--------|------|--------|-------------------------|
| 0x0    | 0x4  | char[] | Magic "LGHT"            |
| 0x4    | 0x4  | u32    | Mask of light LUTs used |
| 0x8    | 0x18 | u32[]  | Offset of each LUT      |

There are 6 LUTs:

| LUT | Description          |
|-----|----------------------|
| D0  | Distribution LUT 0   |
| D1  | Distribution LUT 1   |
| RR  | Reflection Red       |
| RG  | Reflection Green     |
| RB  | Reflection Blue      |
| F   | Fresnel              |

These LUTs are used (along with the materials) to calculate the primary and secondary fragment colours.
The following formula is used to do so:

- Secondary colour = sum of each light colour
- light n colour = spotlight attn * distance attn * (specular 0 + specular 1)
- specular 0 = light n specular 0 * material specular 0 * D0 * Geometry Factor 0
- specular 1 = light n specular 1 * (material specular 1 OR Reflection LUTs (if used)) * D1 * Geometry Factor 1

### Combiner Directives

