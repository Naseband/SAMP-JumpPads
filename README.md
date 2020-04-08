# SAMP-JumpPads
Simple JumpPads that send players flying from A to B.

# JumpPad Types:

JUMP_PAD_TYPE_GHOST
- Ignores collisions and flies through any obstacles (always used if ColAndreas is not included)

JUMP_PAD_TYPE_COLLIDE
- When an object is blocking the path, it will end at the point of collision and the player will fall down, useful if the destination is not an exact location, parabola height depends on the distance between the Pad and the destination.

JUMP_PAD_TYPE_FIND_Z
- When creating a JumpPad of this type, the script will try to find a way around any obstacles by increasing the parabola's height until the height limit is reached. If it fails to generate the JumpPad will not be created.

# Functions

```CreateJumpPad(type, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:dest_x, Float:dest_y, Float:dest_z, Float:speed = JUMP_PAD_DEFAULT_SPEED, worldid = -1, interiorid = -1, playerid = -1, Float:find_surface_dest = -15.0, Float:try_z_min = 5.0, Float:try_z_max = 700.0, Float:try_z_step = 5.0)```

- Returns JPad ID or INVALID_JUMP_PAD if not created.

- find_surface_dest: If this value is not 0.0, it tries to find the nearest surface below or above the destination coords within the given distance (negative for below, positive for above).
- try_z_min, try_z_max, try_z_step: When type is JUMP_PAD_TYPE_FIND_Z, these values determine the minimum/maximum height that is tested and by which distance (step) it increases between each test. Using a small step and a high maximum height can cause the script to be busy for a bit when creating the JPad.

```IsValidJumpPad(jpadid)```

- Returns 1 if valid, 0 otherwise.

```DestroyJumpPad(jpadid)```

- Returns 1 if it was successfully destroyed, 0 otherwise.

```DestroyAllJumpPads()```

- Returns amount of destroyed JPads.
