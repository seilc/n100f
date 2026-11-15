#ifndef ZGLOBALS_H
#define ZGLOBALS_H

#include "iFog.h"
#include "iModel.h"
#include "iTime.h"
#include "xCamera.h"
#include "xCollide.h"
#include "xEntDrive.h"
#include "zEnt.h"
#include "zScene.h"

typedef struct _tagxPad xPad;
typedef struct _zCutsceneMgr zCutsceneMgr;
typedef struct _zEntHangable zEntHangable;
typedef struct _zPlatform zPlatform;
struct zAssetPickupTable;

struct zJumpParam
{
    F32 PeakHeight;
    F32 TimeGravChange;
    F32 TimeHold;
    F32 ImpulseVel;
};

struct zPlayerStatic
{
    U16 AnalogMin;
    U16 AnalogMax;
    F32 SpeedSneak[4];
    F32 SpeedWalk[4];
    F32 SpeedRun[4];
    F32 SpeedAir[4];
    F32 SpeedHotsauce[4];
    F32 AnimSneak[3];
    F32 AnimWalk[3];
    F32 Gravity;
    F32 JumpGravity;
    F32 GravSmooth;
    F32 FloatSpeed;
    F32 ButtsmashSpeed;
    zJumpParam Jump;
    zJumpParam Double;
    zJumpParam Bounce;
    zJumpParam Spring;
    zJumpParam Toss;
    F32 HeadbuttSpeedMin;
    F32 HeadbuttSpeedMax;
    F32 HeadbuttAccel;
    F32 HeadbuttTimeMin;
    F32 HeadbuttTimeMax;
    F32 HeadbuttTurn;
    F32 FireDelay;
    F32 FireRepeat;
    F32 FirePress;
    F32 FireTargetYaw;
    F32 FireTargetPitch;
    F32 FireTargetDist;
    F32 FireTargetRadius;
    F32 FireTrack;
    F32 FireGumSpeed;
    F32 FireSoapSpeed;
    F32 FireGumTimer;
    F32 FireSoapTimer;
    U32 Initial_Specials;
    U32 TakeDamage;
    F32 StartSlideAngle;
    F32 StopSlideAngle;
    F32 RotMatchMaxAngle;
    F32 RotMatchMatchTime;
    F32 RotMatchRelaxTime;
    U16 AmmoCapacityGum;
    U16 AmmoCapacitySoap;
    U16 AmmoSceneGum;
    U16 AmmoSceneSoap;
};

struct zShaggy1Globals
{
    zEnt* GrabTry;
    zEnt* Grab;
    F32 GrabRot;
    F32 GrabOffset;
    F32 GrabTimer;
    xVec3 ReleaseTrans;
    xVec3 HangTrans;
    xVec3 LastDelta;
    U32 ThrowEnded;
    F32 ExtraTransY;
    xVec3 ThrowPos;
    xVec3 ThrowVel;
    U32 CatchMe;
    U32 CatchYou;
    zEnt* HangTarget;
    F32 ThrowTimer;
    xVec3 HangStart;
    xVec3 HangRelative;
    U32 SetDown;
    U32 GrabOn;
    U32 ThrowState;
    F32 ThrowRot;
    F32 CatchHeight;
    F32 CatchVel;
    xBase* ThrowTgt;
    F32 ThrowTgtAngle;
    F32 ThrowTgtPeak;
    F32 ThrowTgtOffset;
    U32 ThrowTgtAdjust;
    xVec3 ThrowTgtVec;
    F32 ThrowTgtSpeed;
    F32 ThrowAccel;
    zEnt* Hangable[10];
};

struct zCheckPoint
{
    xVec3 pos;
    F32 rot;
    U32 initCamID;
};

struct zPlayerGlobals
{
    zEnt ent;
    U32 Visible;
    U32 Health;
    S32 Speed;
    S32 Sneak;
    S32 Teeter;
    S32 CanSlip;
    F32 SlipFadeTimer;
    S32 Slide;
    F32 SlideTimer;
    S32 Stepping;
    S32 JumpState;
    S32 LastJumpState;
    F32 JumpTimer;
    F32 LookAroundTimer;
    U32 LookAroundRand;
    U32 LastProjectile;
    F32 DecelRun;
    F32 DecelRunSpeed;
    F32 HotsauceTimer;
    F32 LeanLerp;
    U32 HeadbuttHit;
    F32 ScareTimer;
    xBase* ScareSource;
    U32 ScareShaggy;
    F32 CowerTimer;
    F32 DamageTimer;
    F32 ControlOffTimer;
    F32 HelmetTimer;
    U32 WorldDisguise;
    U32 Bounced;
    F32 FallDeathTimer;
    F32 HeadbuttVel;
    F32 HeadbuttTimer;
    U32 SpecialReceived;
    xEnt* MountChimney;
    F32 MountChimOldY;
    F32 ShockRadius;
    F32 ShockRadiusOld;
    F32 Face_ScareTimer;
    U32 Face_ScareRandom;
    U32 Face_Event;
    F32 Face_EventTimer;
    F32 Face_PantTimer;
    U32 Face_AnimSpecific;
    U32 IdleRand;
    F32 IdleMinorTimer;
    F32 IdleMajorTimer;
    F32 IdleSitTimer;
    S32 Transparent;
    zEnt* FireTarget;
    zEnt* DamageSource;
    U32 ControlOn;
    U32 ControlOnEvent;
    U32 AutoMoveSpeed;
    F32 AutoMoveDist;
    xVec3 AutoMoveTarget;
    xBase* AutoMoveObject;
    zEnt* Diggable;
    F32 DigTimer;
    zShaggy1Globals Shaggy1;
    zEnt* Shaggy8Call;
    F32 Shaggy8CallTimer;
    zEntHangable* HangFound;
    zEntHangable* HangEnt;
    zEntHangable* HangEntLast;
    xVec3 HangPivot;
    xVec3 HangVel;
    F32 HangLength;
    xVec3 HangStartPos;
    F32 HangStartLerp;
    iModelTag HangPawTag[4];
    F32 HangPawOffset;
    F32 HangElapsed;
    F32 Jump_CurrGravity;
    F32 Jump_HoldTimer;
    F32 Jump_ChangeTimer;
    S32 Jump_CanDouble;
    S32 Jump_CanFloat;
    S32 Jump_SpringboardStart;
    zPlatform* Jump_Springboard;
    S32 cheat_mode;
    U32 Inv_Clue;
    U32 Inv_Key;
    U32 Inv_Chances;
    U32 Inv_ScoobySnacks;
    U32 Inv_Specials;
    U32 Inv_Mtokens[1];
    S32 Inv_Ammo[2];
    S32 Inv_AmmoMax[2];
    iModelTag BubbleTag;
    xEntDrive drv;
    xSurface* floor_surf;
    xVec3 floor_norm;
    S32 slope;
    xCollis earc_coll;
    xSphere head_sph;
    iModelTag center_tag;
    iModelTag head_tag;
    U32 TongueFlags[2];
    xVec3 RootUp;
    xVec3 RootUpTarget;
    zPlayerStatic s;
    zCheckPoint cp;
};

struct zGlobalGameStats
{
    F32 gameTime;
    U32 visited;
    F32 levelTime;
    F32 levelTime_sum;
    F32 areaTime;
    F32 areaTime_sum;
    F32 roomTime;
    F32 roomTime_sum;
    F32 timeRunning;
    F32 timeRunning_sum;
    F32 timeWalking;
    F32 timeWalking_sum;
    U32 scares;
    U32 scares_sum;
    U32 deaths;
    U32 deaths_sum;
    U32 singleJumps;
    U32 singleJumps_sum;
    U32 doubleJumps;
    U32 doubleJumps_sum;
    U32 gumShot;
    U32 gumShot_sum;
    U32 gumHit;
    U32 gumHit_sum;
    U32 gumEmpty;
    U32 gumEmpty_sum;
    U32 soapShot;
    U32 soapShot_sum;
    U32 soapHit;
    U32 soapHit_sum;
    U32 soapEmpty;
    U32 soapEmpty_sum;
    U32 food;
    U32 food_sum;
    U32 snackBoxes;
    U32 snackBoxes_sum;
    U32 enemyKills;
    U32 enemyKills_sum;
    U32 enemyCount;
    U32 enemyCount_sum;
    U32 specials;
    U32 specials_tot;
    U32 tokens;
    U32 tokens_tot;
    U32 snacks;
    U32 snacks_tot;
    U32 chances;
    U32 chances_tot;
    U32 keys;
    U32 keys_tot;
    U32 clues;
    U32 clues_tot;
};

struct zGlobals
{
    xCamFrame camera;
    xPad* pad0;
    xPad* pad1;
    zPlayerGlobals player;
    zAssetPickupTable* pickupTable;
    S32 profile;
    zCutsceneMgr* cmgr;
    char profFunc[6][128];
    zScene* sceneCur;
    zScene* scenePreload;
    S32 sceneFirst;
    char sceneStart[32];
    iFogParams fog;
    iFogParams fogA;
    iFogParams fogB;
    iTime fog_t0;
    iTime fog_t1;
    zGlobalGameStats stats;
    zGlobalGameStats areaStats[30];
    zGlobalGameStats levelStats[13];
    zGlobalGameStats worldStats[4];
    S32 option_vibration;
    F32 update_dt;
};

extern zGlobals globals;

#endif
