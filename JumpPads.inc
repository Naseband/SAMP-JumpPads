#if defined __JUMP_PADS_INCLUDE__
#endinput
#endif
#define __JUMP_PADS_INCLUDE__

/*

JumpPads Include by NaS

Creates Jump Pads that send the player flying to a specified location.
The script will calculate a trajectory when the pad is created. When entering the player will be forced to surf
on an invisible moving object that moves along the path.

*/

// ---------------------------------------------------- // 

// Main Limits

#if !defined MAX_JUMP_PADS
#define MAX_JUMP_PADS				50
#endif

#if !defined MAX_JUMP_PAD_PATH_LEN
#define MAX_JUMP_PAD_PATH_LEN		100 // Not many are required
#endif

// This include uses the Streamer's Extra Array, index 0 for determinig the type and index 1 for the Jump Pad ID

#if !defined JUMP_PAD_STREAMER_EXTRA
#define JUMP_PAD_STREAMER_EXTRA		-3445744
#endif

// Default stuff

#if !defined JUMP_PAD_DEFAULT_SPEED
#define JUMP_PAD_DEFAULT_SPEED		100.0
#endif

#if !defined JUMP_PAD_MIN_SPEED
#define JUMP_PAD_MIN_SPEED			(JUMP_PAD_DEFAULT_SPEED/3.5)
#endif

#if !defined JUMP_PAD_ANIM_ID
#define JUMP_PAD_ANIM_ID			1541 // 1204, 1244, 1379 (sideways) - Some anim IDs to try out ;)
#endif

// Other

#define INVALID_JUMP_PAD			(-1)

// ---------------------------------------------------- // Data, Vars

enum (+=1)
{
	JUMP_PAD_TYPE_GHOST,
	JUMP_PAD_TYPE_COLLIDE,
	JUMP_PAD_TYPE_FIND_Z
};

enum _E_JUMP_PAD
{
	bool:jpadUsed,
	jpadType,
	Float:jpadSpeed,

	Float:jpadPosX,
	Float:jpadPosY,
	Float:jpadPosZ,
	Float:jpadRotX,
	Float:jpadRotY,
	Float:jpadRotZ,

	Float:jpadDestX,
	Float:jpadDestY,
	Float:jpadDestZ,

	Float:jpadPath[MAX_JUMP_PAD_PATH_LEN * 3],
	jpadPathLen,
	Float:jpadPathRZ,

	// Items

	jpadArrowObjID,
	jpadAreaID
};
new static gJumpPads[MAX_JUMP_PADS][_E_JUMP_PAD];


enum _E_JUMP_PAD_PLAYER
{
	jpadPlayerJPID, // Determines whether the player is currently using any Jump Pad, and which one
	jpadPlayerObjID, // Player Object ID of the object the player is surfing on
	jpadPlayerPathNode
};
new static gJumpPadPlayers[MAX_PLAYERS][_E_JUMP_PAD_PLAYER];

new static bool:gJPADInit = false;

// ---------------------------------------------------- // Hooks

public OnFilterScriptInit()
{
	JPAD_OnInit();


	#if defined JPAD_OnFilterScriptInit
		return JPAD_OnFilterScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit JPAD_OnFilterScriptInit
#if defined JPAD_OnFilterScriptInit
	forward JPAD_OnFilterScriptInit();
#endif


public OnFilterScriptExit()
{
	JPAD_OnExit();


	#if defined JPAD_OnFilterScriptExit
		return JPAD_OnFilterScriptExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif

#define OnFilterScriptExit JPAD_OnFilterScriptExit
#if defined JPAD_OnFilterScriptExit
	forward JPAD_OnFilterScriptExit();
#endif


public OnGameModeInit()
{
	JPAD_OnInit();


	#if defined JPAD_OnGameModeInit
		return JPAD_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit JPAD_OnGameModeInit
#if defined JPAD_OnGameModeInit
	forward JPAD_OnGameModeInit();
#endif


public OnGameModeExit()
{
	JPAD_OnExit();


	#if defined JPAD_OnGameModeExit
		return JPAD_OnGameModeExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif

#define OnGameModeExit JPAD_OnGameModeExit
#if defined JPAD_OnGameModeExit
	forward JPAD_OnGameModeExit();
#endif


public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid)
{
	if(gJPADInit)
	{
		JPAD_EnterArea(playerid, areaid);
	}


	#if defined JPAD_OnPlayerEnterDynamicArea
		return JPAD_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif

#define OnPlayerEnterDynamicArea JPAD_OnPlayerEnterDynamicArea
#if defined JPAD_OnPlayerEnterDynamicArea
	forward JPAD_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA areaid);
#endif


public OnPlayerObjectMoved(playerid, objectid)
{
	if(gJPADInit)
	{
		JPAD_ObjectMoved(playerid, objectid);
	}


	#if defined JPAD_OnPlayerObjectMoved
		return JPAD_OnPlayerObjectMoved(playerid, objectid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerObjectMoved
	#undef OnPlayerObjectMoved
#else
	#define _ALS_OnPlayerObjectMoved
#endif

#define OnPlayerObjectMoved JPAD_OnPlayerObjectMoved
#if defined JPAD_OnPlayerObjectMoved
	forward JPAD_OnPlayerObjectMoved(playerid, objectid);
#endif


public OnPlayerConnect(playerid)
{
	JPAD_PlayerConnect(playerid);

	
	#if defined JPAD_OnPlayerConnect
		return JPAD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect JPAD_OnPlayerConnect
#if defined JPAD_OnPlayerConnect
	forward JPAD_OnPlayerConnect(playerid);
#endif

// ---------------------------------------------------- // Internal

static JPAD_OnInit()
{
	if(gJPADInit) return 0;

	for(new i = 0; i < MAX_PLAYERS; i ++) if(IsPlayerConnected(i)) JPAD_PlayerConnect(i);

	gJPADInit = true;

	return 1;
}

static JPAD_OnExit()
{
	if(!gJPADInit) return 0;

	DestroyAllJumpPads();

	gJPADInit = false;

	return 1;
}

static JPAD_PlayerConnect(playerid)
{
	gJumpPadPlayers[playerid][jpadPlayerJPID] = INVALID_JUMP_PAD;
	gJumpPadPlayers[playerid][jpadPlayerObjID] = -1;

	return 1;
}

static JPAD_EnterArea(playerid, areaid)
{
	new jpadid = JPAD_GetAreaExtra(areaid);

	if(IsValidJumpPad(jpadid) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		if(gJumpPadPlayers[playerid][jpadPlayerJPID] == INVALID_JUMP_PAD)
		{
			new Float:x = gJumpPads[jpadid][jpadPosX], Float:y = gJumpPads[jpadid][jpadPosY], Float:z = gJumpPads[jpadid][jpadPosZ];

			// Prepare player for the flight

			SetPlayerPos(playerid, x, y, z);
			SetPlayerFacingAngle(playerid, gJumpPads[jpadid][jpadPathRZ]);
			SetCameraBehindPlayer(playerid);
			//TogglePlayerControllable(playerid, 0);
			
			new objectid = CreatePlayerObject(playerid, 19789, x, y, z - 2.0, 0.0, 0.0, 0.0, 900.0);

			SetPlayerObjectMaterialText(playerid, objectid, " ", 0); // Make it invisible
			gJumpPadPlayers[playerid][jpadPlayerObjID] = objectid;

			gJumpPadPlayers[playerid][jpadPlayerJPID] = jpadid;
			gJumpPadPlayers[playerid][jpadPlayerPathNode] = 0;

			MovePlayerObject(playerid, objectid, x, y, z - 1.0, 2.0); // Moves the platform away from the ground, this ensures the player will actually surf on the object!

			return 1;
		}
	}

	return 0;
}

static JPAD_ObjectMoved(playerid, objectid)
{
	new jpadid = gJumpPadPlayers[playerid][jpadPlayerJPID];

	if(IsValidJumpPad(jpadid) && gJumpPadPlayers[playerid][jpadPlayerObjID] == objectid)
	{
		if(!JPAD_Move(jpadid, playerid))
		{
			gJumpPadPlayers[playerid][jpadPlayerJPID] = INVALID_JUMP_PAD;
			DestroyPlayerObject(playerid, objectid);
			gJumpPadPlayers[playerid][jpadPlayerObjID] = -1;

			//TogglePlayerControllable(playerid, 1);
			if(GetPlayerAnimationIndex(playerid) == JUMP_PAD_ANIM_ID) ClearAnimations(playerid);
		}

		return 1;
	}

	return 0;
}

static JPAD_Move(jpadid, playerid)
{
	if(gJumpPadPlayers[playerid][jpadPlayerPathNode] < gJumpPads[jpadid][jpadPathLen])
	{
		new nodeid = gJumpPadPlayers[playerid][jpadPlayerPathNode] * 3;

		new animlib[33], animname[33];
		GetAnimationName(JUMP_PAD_ANIM_ID, animlib, sizeof animlib, animname, sizeof animname);

		new Float:speed = JUMP_PAD_MIN_SPEED;

		if(nodeid >= 3) // This slows down the object a bit when it moves rather horizontally
		{
			new Float:vz = gJumpPads[jpadid][jpadPath][nodeid-1] - gJumpPads[jpadid][jpadPath][nodeid+2],
				Float:vlen = VectorSize(gJumpPads[jpadid][jpadPath][nodeid-3] - gJumpPads[jpadid][jpadPath][nodeid], gJumpPads[jpadid][jpadPath][nodeid-2] - gJumpPads[jpadid][jpadPath][nodeid+1], vz);

			if(vlen)
			{
				vz = floatabs((vz / vlen) * gJumpPads[jpadid][jpadSpeed]);

				if(vz > JUMP_PAD_MIN_SPEED) speed = vz;
			}
		}

		MovePlayerObject(playerid, gJumpPadPlayers[playerid][jpadPlayerObjID], gJumpPads[jpadid][jpadPath][nodeid], gJumpPads[jpadid][jpadPath][nodeid + 1], gJumpPads[jpadid][jpadPath][nodeid + 2], speed);

		ApplyAnimation(playerid, animlib, animname, 4.1, 1, 1, 1, 0, 0);

		gJumpPadPlayers[playerid][jpadPlayerPathNode] ++;

		return 1;
	}

	return 0;
}

static stock JPAD_SetAreaExtra(areaid, jpadid)
{
	new extra_array[2] = {JUMP_PAD_STREAMER_EXTRA, 0};
	extra_array[1] = jpadid;

	return Streamer_SetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, extra_array);
}

static stock JPAD_GetAreaExtra(areaid)
{
	new extra_array[2];

	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, extra_array);

	if(extra_array[0] == JUMP_PAD_STREAMER_EXTRA) return extra_array[1];

	return INVALID_JUMP_PAD;
}

stock static Float:JPAD_RetRXFromVectorZ(Float:vz)
{
	return -(acos(vz) - 90.0);
}

stock static Float:JPAD_RetRZFromVectorXY(Float:vx, Float:vy)
{
	if(vx == 0.0 && vy == 0.0) return 0.0;

	return atan2(vy, vx) - 90.0;
}

// Gets the coefficients for a cubic parabola, I use the distance (gta x & y axes) for x and height (gta z axis) for y

stock static JPAD_GetCoefficientsForPoints(Float:x_1, Float:y_1, Float:x_2, Float:y_2, Float:x_3, Float:y_3, &Float:a, &Float:b, &Float:c)
{
	a = y_1/((x_1-x_2)*(x_1-x_3)) + y_2/((x_2-x_1)*(x_2-x_3)) + y_3/((x_3-x_1)*(x_3-x_2));

	b = -y_1*(x_2+x_3)/((x_1-x_2)*(x_1-x_3)) - y_2*(x_1+x_3)/((x_2-x_1)*(x_2-x_3)) - y_3*(x_1+x_2)/((x_3-x_1)*(x_3-x_2));

	c = y_1*x_2*x_3/((x_1-x_2)*(x_1-x_3)) + y_2*x_1*x_3/((x_2-x_1)*(x_2-x_3)) + y_3*x_1*x_2/((x_3-x_1)*(x_3-x_2));

	return 1;
}

// ---------------------------------------------------- // External

IsValidJumpPad(jpadid)
{
	if(jpadid < 0 || jpadid >= MAX_JUMP_PADS) return 0;

	return _:gJumpPads[jpadid][jpadUsed];
}

CreateJumpPad(type, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:dest_x, Float:dest_y, Float:dest_z, Float:speed = JUMP_PAD_DEFAULT_SPEED, worldid = -1, interiorid = -1, playerid = -1, Float:find_surface_dest = -15.0, Float:try_z_min = 5.0, Float:try_z_max = 700.0, Float:try_z_step = 5.0)
{
	#pragma unused try_z_min
	#pragma unused try_z_max
	#pragma unused try_z_step
	#pragma unused find_surface_dest

	if(!gJPADInit) return INVALID_JUMP_PAD;

	new jpadid = INVALID_JUMP_PAD;

	for(new i = 0; i < MAX_JUMP_PADS; i ++) if(!IsValidJumpPad(i))
	{
		jpadid = i;
		break;
	}

	if(jpadid == INVALID_JUMP_PAD) return INVALID_JUMP_PAD;

	// Surface finding is obsolete in mode 0

	#if defined CA_CreateObject

	if(find_surface_dest != 0.0) CA_RayCastLine(dest_x, dest_y, dest_z + 0.5, dest_x, dest_y, dest_z + find_surface_dest, dest_x, dest_y, dest_z);

	#endif

	new Float:vx = dest_x - x,
		Float:vy = dest_y - y,
		Float:vz = dest_z - z,
		Float:vlen = VectorSize(vx, vy, vz),
		Float:vlen2 = VectorSize(vx, vy, 0.0);

	vx = vx / vlen;
	vy = vy / vlen;
	vz = vz / vlen;

	new Float:path_rx = JPAD_RetRXFromVectorZ(vz),
		Float:path_rz = JPAD_RetRZFromVectorXY(vx, vy);

	new num = MAX_JUMP_PAD_PATH_LEN,
		Float:step = vlen2 / MAX_JUMP_PAD_PATH_LEN; // Step must be 2D!!

	if(step < 1.5)
	{
		num = floatround(vlen / 1.5, floatround_floor);

		step = vlen2 / num;
	}

	new count, Float:a, Float:b, Float:c;

	#if !defined CA_CreateObject // ----------------------------- No Colandreas

		JPAD_GetCoefficientsForPoints(-(vlen2 / 2.0), z, 0.0, (z < dest_z ? dest_z : z) + (vlen2 / 4.0), (vlen2 / 2.0), dest_z, a, b, c);

		for(new i = 1; i <= num; i ++)
		{
			new nodeid = count * 3,
				Float:tx = x + i * step * floatsin(-path_rz, degrees),
				Float:ty = y + i * step * floatcos(-path_rz, degrees),
				Float:curve_x = -(vlen2 / 2.0) + i * step,
				Float:tz = a * (curve_x * curve_x) + b * curve_x + c;

			gJumpPads[jpadid][jpadPath][nodeid] = tx;
			gJumpPads[jpadid][jpadPath][nodeid + 1] = ty;
			gJumpPads[jpadid][jpadPath][nodeid + 2] = tz;

			count ++;
		}

		type = JUMP_PAD_TYPE_GHOST

	#else // ---------------------------------------------------- ColAndreas

		switch(type)
		{
			case JUMP_PAD_TYPE_GHOST:
			{
				JPAD_GetCoefficientsForPoints(-(vlen2 / 2.0), z, 0.0, (z < dest_z ? dest_z : z) + (vlen2 / 4.0), (vlen2 / 2.0), dest_z, a, b, c);

				for(new i = 1; i <= num; i ++)
				{
					new nodeid = count * 3,
						Float:tx = x + i * step * floatsin(-path_rz, degrees),
						Float:ty = y + i * step * floatcos(-path_rz, degrees),
						Float:curve_x = -(vlen2 / 2.0) + i * step,
						Float:tz = a * (curve_x * curve_x) + b * curve_x + c;

					gJumpPads[jpadid][jpadPath][nodeid] = tx;
					gJumpPads[jpadid][jpadPath][nodeid + 1] = ty;
					gJumpPads[jpadid][jpadPath][nodeid + 2] = tz;

					count ++;
				}
			}
			case JUMP_PAD_TYPE_COLLIDE:
			{
				new Float:lastx, Float:lasty, Float:lastz;

				JPAD_GetCoefficientsForPoints(-(vlen2 / 2.0), z, 0.0, (z < dest_z ? dest_z : z) + (vlen2 / 4.0), (vlen2 / 2.0), dest_z, a, b, c);

				for(new i = 1; i <= num; i ++)
				{
					new nodeid = count * 3,
						Float:tx = x + i * step * floatsin(-path_rz, degrees),
						Float:ty = y + i * step * floatcos(-path_rz, degrees),
						Float:curve_x = -(vlen2 / 2.0) + i * step,
						Float:tz = a * (curve_x * curve_x) + b * curve_x + c;

					if(count && CA_RayCastLine(tx, ty, tz, lastx, lasty, lastz, tx, ty, tz) != 0) // Ouch!
					{
						gJumpPads[jpadid][jpadPath][nodeid] = tx;
						gJumpPads[jpadid][jpadPath][nodeid + 1] = ty;
						gJumpPads[jpadid][jpadPath][nodeid + 2] = tz + 0.5;

						count ++;

						break;
					}

					lastx = gJumpPads[jpadid][jpadPath][nodeid] = tx;
					lasty = gJumpPads[jpadid][jpadPath][nodeid + 1] = ty;
					lastz = gJumpPads[jpadid][jpadPath][nodeid + 2] = tz;

					count ++;
				}

			}
			case JUMP_PAD_TYPE_FIND_Z:
			{
				new Float:try_z = (z < dest_z ? dest_z : z) + try_z_min, Float:try_z_d = try_z_step,
					tries;

				do
				{
					JPAD_GetCoefficientsForPoints(-(vlen2 / 2.0), z, 0.0, try_z, (vlen2 / 2.0), dest_z, a, b, c);

					try_z += try_z_d;
					try_z_d += try_z_d * 0.1;

					for(new i = 1; i <= num; i ++)
					{
						new nodeid = count * 3,
							Float:tx = x + i * step * floatsin(-path_rz, degrees),
							Float:ty = y + i * step * floatcos(-path_rz, degrees),
							Float:curve_x = -(vlen2 / 2.0) + i * step,
							Float:tz = a * (curve_x * curve_x) + b * curve_x + c;

						if(count && CA_RayCastLine(tx, ty, tz, gJumpPads[jpadid][jpadPath][nodeid - 3], gJumpPads[jpadid][jpadPath][nodeid - 2], gJumpPads[jpadid][jpadPath][nodeid - 1], tx, ty, tz) != 0) // Nope
						{
							count = 0;

							break;
						}

						gJumpPads[jpadid][jpadPath][nodeid] = tx;
						gJumpPads[jpadid][jpadPath][nodeid + 1] = ty;
						gJumpPads[jpadid][jpadPath][nodeid + 2] = tz;

						count ++;
					}

					tries ++;
				}
				while(!count && try_z < try_z_max);
			}
		}

	#endif // ca else

	if(!count) return INVALID_JUMP_PAD;

	gJumpPads[jpadid][jpadPosX] = x;
	gJumpPads[jpadid][jpadPosY] = y;
	gJumpPads[jpadid][jpadPosZ] = z;
	gJumpPads[jpadid][jpadRotX] = rx;
	gJumpPads[jpadid][jpadRotY] = ry;
	gJumpPads[jpadid][jpadRotZ] = rz;

	gJumpPads[jpadid][jpadDestX] = dest_x;
	gJumpPads[jpadid][jpadDestY] = dest_y;
	gJumpPads[jpadid][jpadDestZ] = dest_z;

	gJumpPads[jpadid][jpadType] = type;
	gJumpPads[jpadid][jpadSpeed] = speed;

	gJumpPads[jpadid][jpadPathLen] = count;
	gJumpPads[jpadid][jpadPathRZ] = path_rz;

	gJumpPads[jpadid][jpadArrowObjID] = CreateDynamicObject(19130, x, y, z, 0.0, path_rx - 270.0, path_rz - 90.0, worldid, interiorid, playerid, 100.0, 100.0);

	new areaid = CreateDynamicSphere(x, y, z, 1.25, worldid, interiorid, playerid);
	JPAD_SetAreaExtra(areaid, jpadid);
	gJumpPads[jpadid][jpadAreaID] = areaid;

	gJumpPads[jpadid][jpadUsed] = true;

	return jpadid;
}

DestroyJumpPad(jpadid)
{
	if(!IsValidJumpPad(jpadid)) return 0;

	DestroyDynamicObject(gJumpPads[jpadid][jpadArrowObjID]);
	DestroyDynamicArea(gJumpPads[jpadid][jpadAreaID]);

	for(new playerid; playerid < MAX_PLAYERS; playerid ++) if(IsPlayerConnected(playerid))
	{
		if(gJumpPadPlayers[playerid][jpadPlayerJPID] == jpadid)
		{
			gJumpPadPlayers[playerid][jpadPlayerJPID] = INVALID_JUMP_PAD;
			DestroyPlayerObject(playerid, gJumpPadPlayers[playerid][jpadPlayerObjID]);
			gJumpPadPlayers[playerid][jpadPlayerObjID] = -1;

			ClearAnimations(playerid);
		}
	}

	gJumpPads[jpadid][jpadUsed] = false;

	return 1;
}

DestroyAllJumpPads()
{
	new c;
	for(new i = 0; i < MAX_JUMP_PADS; i ++) if(DestroyJumpPad(i)) c ++;

	return c;
}

// ---------------------------------------------------- // 

// EOF
