# Animations

Animation files have the extension .qlska, and contain skeletal animations.
Each file can store a single animation, which is described via skeleton keyframes.
An animation is not specific to a single mesh and can be used on any deformable mesh (as long as the IDs match of course). 
There can be up to 40 bones in a mesh, and up to 4 bones can affect a given vertex. 

## Header

| Offset | Size | Type   | Description         |
|--------|------|--------|---------------------|
| 0x0    | 0x4  | char[] | Magic "ANIM"        |
| 0x4    | 0x4  | u32    | Number of keyframes |
| 0x8    | 0x4  | u32    | Flags               |

## Bones

Bones are stored in dual-quaternion format, along with the parent ID. 

| Offset | Size | Type  | Description |
|--------|------|-------|-------------|
| 0x0    | 0x2  | u16   | Bone ID     |
| 0x2    | 0x2  | u16   | Parent ID   |
| 0x4    | 0x32 | FQuat | Quat 1      |
| 0x24   | 0x32 | FQuat | Quat 2      |

## Keyframes

Each frame of the animation includes a full skeleton, duration, and interpolation function with 3 arbitrary parameters.

Format:
| Offset | Size   | Description                       |
|--------|--------|-----------------------------------|
| 0x0    | 0x4    | Frame ID                          |
| 0x4    | 0x4    | Frame Duration (float)            |
| 0x8    | 0x4    | Interpolation type                |
| 0xa    | 0x4*3  | Parameters (float)                |
| 0x18   | 0x4    | Skeleton Offset (from file start) |