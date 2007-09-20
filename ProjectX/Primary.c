 

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*	p r i m a r y . c
*	All routines to do with Primary weapons...
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include "main.h"

#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"


#include "d3dmain.h"
#include "typedefs.h"
#include "New3D.h"
#include "Ships.h"
#include "exechand.h"
#include "Mload.h"
#include "collision.h"
#include "lights.h"
#include "2dpolys.h"
#include "polys.h"
#include "lines.h"
#include "sphere.h"
#include "controls.h"
#include "visi.h"
#include "2dtextures.h"
#include "sfx.h"

#include "triggers.h"
#include "trigarea.h"
#include "pickups.h"

#include "enemies.h"

#include "Primary.h"
#include "Secondary.h"

#include "spotfx.h"
#include "title.h"
#include "text.h"

#include "AI.h"
#include "Water.h"

#ifdef SHADOWTEST
#include "shadows.h"
#endif

#include "local.h"

#if !defined( VERSION_SPANISH ) && !defined( VERSION_ITALIAN ) && !defined( VERSION_GERMAN ) && !defined( VERSION_FRENCH )
#include "localtitle.h"
#elif defined( VERSION_SPANISH )
#include "sp-localtitle.h"
#elif defined( VERSION_ITALIAN )
#include "it-localtitle.h"
#elif defined( VERSION_GERMAN )
#include "ge-localtitle.h"
#elif defined( VERSION_FRENCH )
#include "fr-localtitle.h"
#endif

#ifdef OPT_ON
#pragma optimize( "gty", on )
#endif

#define MAX_TROJAX_POWER_LEVEL	(100.0F)

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Externs
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	BYTE	Server_WhoHitYou;
extern SLIDER BikeCompSpeechSlider;
extern	BOOL	BombTag;
extern	int		LowestBombTime;

extern	SHIPCONTROL		control;
extern	D3DMATRIX		view;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	D3DMATERIALHANDLE hMat2;
extern	D3DTEXTUREHANDLE bTex;
extern	MLOADHEADER		Mloadheader;
extern	MCLOADHEADER	MCloadheader;
extern	MCLOADHEADER	MCloadheadert0;
extern	XLIGHT			XLights[MAXXLIGHTS];
extern	FMPOLY			FmPolys[MAXNUMOF2DPOLYS];
extern	POLY			Polys[MAXPOLYS];
extern	LINE			Lines[ MAXLINES ];
extern	float			framelag;
extern	GLOBALSHIP      Ships[MAX_PLAYERS+1];
extern	BYTE			WhoIAm;
extern	BYTE			Current_Camera_View;		// which object is currently using the camera view....
extern	MODEL			Models[ MAXNUMOFMODELS ];
extern	D3DMATRIX		identity;
extern	uint16			FirstSecBullUsed;
extern	SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern	int16			SecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
extern	VECTOR			ShieldVerts[ 4 ];
extern	int16			SecondaryAmmo[ MAXSECONDARYWEAPONS ];
extern	int16			SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	USERCONFIG	*	player_config;
extern	float			NitroFuel;
extern	float			NitroFuelUsed;
extern	BOOL			PrimaryLightDetail;
extern	BOOL			SecondaryLightDetail;
extern	BOOL            bSoundEnabled;
extern  BOOL			LightningLaser; // user chooses normal laser or lightning from visuals

extern	FRAME_INFO	*	Pulsar_Header;
extern	FRAME_INFO	*	Pulsar_Trail_Header;
extern	FRAME_INFO	*	Pulsar_Exp_Header;
extern	FRAME_INFO	*	Trojax1_Header;
extern	FRAME_INFO	*	Trojax2_Header;
extern	FRAME_INFO	*	Trojax_Exp_Header;
extern	FRAME_INFO	*	Exp_Header;
extern	FRAME_INFO	*	Exp2_Header;
extern	FRAME_INFO	*	Shrapnel_Header;
extern	FRAME_INFO	*	Arc_Header;
extern	FRAME_INFO	*	Flare_Header;
extern	FRAME_INFO	*	Laser_Header;
extern	FRAME_INFO	*	Bits_Header;
extern	FRAME_INFO	*	Circle_Header;
extern	FRAME_INFO	*	Multiple_Header;
extern	FRAME_INFO	*	NMEBullet_Header;
extern	FRAME_INFO	*	Mushroom_Header;
extern	FRAME_INFO	*	BigExp1_Header;

extern BYTE				GameStatus[MAX_PLAYERS];	// Game Status for every Ship...
													// this tells the drones what status the host thinks hes in..
extern	BOOL			DebugInfo;
extern	BOOL			GodMode;
extern	int				no_collision;		// disables player ship-to-background collisions
extern	int16			NumOrbs;
extern	uint16			IsGroupVisible[MAXGROUPS];
extern	SLIDER			TrailDetailSlider;
extern	BOOL			AutoDetail;
extern	float			avgframelag;
extern	int16			NumSuperNashrams;
extern	BOOL			PickupInvulnerability;
extern	int16			NumInvuls;

extern	COMP_OBJ	*	ColChild;
extern	MODELNAME		ModelNames[MAXMODELHEADERS];
extern	float			SoundInfo[MAXGROUPS][MAXGROUPS];
extern	ENEMY			Enemies[ MAXENEMIES ];
extern	ENEMY_TYPES		EnemyTypes[ MAX_ENEMY_TYPES ];
extern	int16			NumSecBullsPerGroup[ MAXGROUPS ];
extern	SECONDARYWEAPONBULLET *	SecBullGroups[ MAXGROUPS ];
extern	BOOL            IsHost;
extern	MXLOADHEADER	ModelHeaders[MAXMODELHEADERS];
extern	MXALOADHEADER	MxaModelHeaders[ MAXMXAMODELHEADERS ];
extern	int				outside_map;
extern	BOOL			PlayDemo;

extern	BOOL	IsServerGame;
extern	BOOL	IsServer;
extern	int16	Host_PrimaryWeaponsGot[ MAX_PLAYERS ][ MAXPRIMARYWEAPONS ];
extern	int16	Host_SecondaryWeaponsGot[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	float	Host_GeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmo[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmo[ MAX_PLAYERS ];
//extern	float	Host_GeneralAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_GeneralAmmoUsed;
//extern	float	Host_PyroliteAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_PyroliteAmmoUsed;
//extern	float	Host_SussGunAmmoUsed[ MAX_PLAYERS ];
extern	float	Host_SussGunAmmoUsed;
extern	int16	Host_SecondaryAmmo[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
//extern	int16	Host_SecAmmoUsed[ MAX_PLAYERS ][ MAXSECONDARYWEAPONS ];
extern	int16	Host_SecAmmoUsed[ MAXSECONDARYWEAPONS ];
extern	int8	Host_SecWeaponsGot[ MAX_PLAYERS ][ MAXPICKUPS ];
extern	float	Host_NitroFuel[ MAX_PLAYERS ];
//extern	float	Host_NitroFuelUsed[ MAX_PLAYERS ];
extern	float	Host_NitroFuelUsed;
extern	int16	Host_PowerLevel[ MAX_PLAYERS ];
extern	float	Host_OrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	BYTE	Host_NumOfOrbitals[ MAX_PLAYERS ];
extern	BYTE	Host_CopyOfNumOfOrbitals[ MAX_PLAYERS ];
extern	float	Host_CopyOfOrbAmmo[ MAX_PLAYERS ][ MAXMULTIPLES ];
extern	int16	Host_CopyOfPowerLevel[ MAX_PLAYERS ];
extern	float	Host_CopyOfNitroFuel[ MAX_PLAYERS ];
extern	float	Host_CopyOfGeneralAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfSussGunAmmo[ MAX_PLAYERS ];
extern	float	Host_CopyOfPyroliteAmmo[ MAX_PLAYERS ];
extern	int16	Host_PickupsGot[ MAX_PLAYERS ][ MAXPICKUPTYPES ];
extern  uint32  Host_Flags[ MAX_PLAYERS ];
extern  float   Host_BountyTimer;
extern  float   Host_FlagTimer;
extern  float   Host_TeamFlagTimer[ MAX_TEAMS ];

#ifdef SINT_PEACEFROG_CHEAT
extern	int16	TargetingWeaponCheat;
#endif

void DebugPrintf( const char * format, ... );
														
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Globals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

#define	SUPERNASHRAM_RED	96
#define	SUPERNASHRAM_GREEN	96
#define	SUPERNASHRAM_BLUE	255

#define	LASER_MINDAMAGERANGE	( ( 10.0F * 1024.0F ) * GLOBAL_SCALE )

float	NmeDamageModifier = 0.75F;

PRIMARYWEAPONBULLET	PrimBulls[MAXPRIMARYWEAPONBULLETS];
uint16	FirstPrimBullUsed;
uint16	FirstPrimBullFree;
float	PrimaryFireDelay = 0.0F;
float	OrbitFireDelay = 0.0F;
float	LaserDiameter = ( 40.0F * GLOBAL_SCALE );
int8	PowerString[ 20 ];
int8	LaserString[ 20 ];
float	PyroliteAmmo = 0.0F;	//MAXPYROLITEAMMO;
float	SussGunAmmo = 0.0F;		//MAXSUSSGUNAMMO;
float	GeneralAmmo = MAXGENERALAMMO;

float	GeneralAmmoUsed = 0.0F;
float	PyroliteAmmoUsed = 0.0F;
float	SussGunAmmoUsed = 0.0F;

float	GlobalFramelagAddition = 0.0F;

float	LaserTemperature = 0.0F;
int16	LaserOverheated = 0;
float	PowerLevel;
static	int16	Toggle = 0;
int16	PrimaryWeaponsGot[ MAXPRIMARYWEAPONS ];
float	SussgunTable[ MAXPOWERLEVELS ] = { 30720.0F, 25600.0F, 20480.0F }; //( 20480.0F + ( ( 5 - PrimBulls[ i ].PowerLevel ) * 2560.0F ) );
uint16	GlobalPrimBullsID = 0;
int32	ColPerspective = COLPERS_Descent;
BOOL	GodModeOnceOnly = TRUE;
BOOL	PyroLightFlag = TRUE;

int8	*	DebugPrimStrings[] = {

			"Pulsar",
			"Trojax",
			"Pyrolite",
			"Transpulse",
			"SussGun",
			"Laser",
			"Orbit Pulsar",
			"Nme Bullet",
			"Nme Pulsar",
			"Nme Trojax",
			"Nme Pyrolite",
			"Nme Transpulse",
			"Nme SussGun",
			"Nme Laser",
			"Nme Lightning",
			"Flames",
			"Nme PowerLaser",

};

uint32 TrojaxChargeID = 0;
uint32 TrojaxOwnersID[ MAX_PLAYERS ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*
char *PrimaryDescription[MAXPRIMARYWEAPONS] =
{ "Pulsar", "Trojax", "Pyrolite", "Transpulse", "Suss Gun", "Laser" };
*/
char *PrimaryDescription[MAXPRIMARYWEAPONS] =
{
	LT_MENU_NEW_PrimaryWeaponKeys2/*"Pulsar"*/,
	LT_MENU_NEW_PrimaryWeaponKeys3/*trojax*/,
	LT_MENU_NEW_PrimaryWeaponKeys4/*Pyrolite*/, 
	LT_MENU_NEW_PrimaryWeaponKeys5 /*"transpulse"*/,
	LT_MENU_NEW_PrimaryWeaponKeys6 /*"suss gun"*/,
	LT_MENU_NEW_PrimaryWeaponKeys7 /*"laser"*/,
};

// is weapon valid as far as order processing & keydefs concerned?
BOOL PrimaryValid[ MAXPRIMARYWEAPONS ] =
{

	TRUE,	// pulsar
	TRUE,	// trojax
	TRUE,	// pyrolite
	TRUE,	// transpulse
	TRUE,	// suss gun
	TRUE,	// laser

};

int16 PrimarySfxLookup[ MAXPRIMARYWEAPONS ] = {

	SFX_Select_Pulsar,
	SFX_Select_Trojax,
	SFX_Select_Pyrolite,
	SFX_Select_Transpulse,
	SFX_Select_SussGun,
	SFX_Select_BeamLaser,
};

int8 PrimaryToFireLookup[ MAXPRIMARYWEAPONS ] = {

	PULSAR,
	TROJAX,
	PYROLITE_RIFLE,
	TRANSPULSE_CANNON,
	SUSS_GUN,
	LASER,
};

int8 PrimaryToFireLookup2[ MAXPRIMARYWEAPONS ] = {

	PULSAR,
	TROJAX,
	PYROLITE_RIFLE,
	TRANSPULSE_CANNON,
	SUSS_GUN,
	NME_LIGHTNING,
};

BOOL	PrimaryWeaponCheat = FALSE;

PRIMARYWEAPONATTRIB	PrimaryWeaponAttribs[ TOTALPRIMARYWEAPONS ] = {

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 0, Pulsar
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move
		1000.0F,											// float	LifeCount
		{ 13, 16, 10 },										// uint16	FireDelay
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 30.0F, 31.0F, 32.0F },							// float	Damage
		{ PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize
		{ 0.0F, 0.0F, 0.0F },								// float	Light red value
		{ 108.0F, 180.0F, 255.0F },							// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value
		FM_PULSAR,											// int16	FmSeq;
		&Pulsar_Header										// FRAME_INFO ** FmFrmInfo
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 1, Trojax
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 0, 0, 0 },										// uint16	FireDelay         
		{ 1.0F, 2.0F, 3.0F },								// float	AmmoUsage         
		{ 160.0F, 160.0F, 256.0F },							// float	Damage            
		{ TROJAX_COLRADIUS,									// float	Collision Radius
		  TROJAX_COLRADIUS,									// float	Collision Radius
		  TROJAX_COLRADIUS },								// float	Collision Radius
		COLTYPE_Trojax,										// float	Collision Type
		( 2048.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 100.0F, 100.0F, 100.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 255.0F, 255.0F, 255.0F },							// float	Light blue value  
		FM_TROJAX,											// int16	FmSeq;            
		&Trojax1_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 2, Pyrolite Rifle
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		64.0F,												// float	LifeCount         
		{ 2, 2, 2 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage         
		{ 2.0F, 4.0F, 6.0F },								// float	Damage            
		{ PYROLITE_COLRADIUS,								// float	Collision Radius
		  PYROLITE_COLRADIUS,								// float	Collision Radius
		  PYROLITE_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 128.0F, 128.0F, 128.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 16.0F, 16.0F, 16.0F },							// float	Light blue value  
		FM_FLAME,											// int16	FmSeq;            
		&Exp2_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 3, Transpulse Cannon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 192.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 192.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 192.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 27, 21, 15 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage         
		{ 45.0F, 55.0F, 64.0F },							// float	Damage            
		{ TRANSPULSE_COLRADIUS,								// float	Collision Radius
		  TRANSPULSE_COLRADIUS,								// float	Collision Radius
		  TRANSPULSE_COLRADIUS },							// float	Collision Radius
		COLTYPE_Transpulse,									// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 108.0F, 180.0F, 255.0F },							// float	Light red value
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value
		FM_ARC,												// int16	FmSeq;            
		&Arc_Header											// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 4, Suss Gun
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 9, 7, 5 },										// uint16	FireDelay         
		{ 1.0F, 2.0F, 3.0F },								// float	AmmoUsage
		{ 11.0F, 13.0F, 15.0F },							// float	Damage            
		{ SUSSGUN_COLRADIUS,								// float	Collision Radius
		  SUSSGUN_COLRADIUS,								// float	Collision Radius
		  SUSSGUN_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 84.0F, 168.0F, 255.0F },							// float	Light red value
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Flare_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 5, Laser
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{ MaxColDistance,									// float	Speed i can move  
		  MaxColDistance,									// float	Speed i can move  
		  MaxColDistance },									// float	Speed i can move  
		12.0F,												// float	LifeCount         
		{ 26, 22, 18 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 5.6F, 5.8F, 6.0F },								// float	Damage            
		{ LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 255.0F, 255.0F, 255.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Laser_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 6, Orbit Pulsar
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move
		1000.0F,											// float	LifeCount
		{ 13, 16, 10 },										// uint16	FireDelay
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 30.0F, 31.0F, 32.0F },							// float	Damage
		{ PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize
		{ 108.0F, 180.0F, 255.0F },							// float	Light red value
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value
		FM_PULSAR,											// int16	FmSeq;
		&Multiple_Header									// FRAME_INFO ** FmFrmInfo
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 7, Nme Bullet
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 64.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 64.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 64.0F * GLOBAL_SCALE ) },						// float	Speed i can move
		1000.0F,											// float	LifeCount
		{ 13, 16, 10 },										// uint16	FireDelay
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 30.0F, 31.0F, 32.0F },							// float	Damage
		{ NMEBUL1_COLRADIUS,								// float	Collision Radius
		  NMEBUL1_COLRADIUS,								// float	Collision Radius
		  NMEBUL1_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize
		{ 108.0F, 180.0F, 255.0F },							// float	Light red value
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value
		{ 50.0F, 50.0F, 50.0F },							// float	Light blue value
		FM_NMEBULLET1,										// int16	FmSeq;
		&NMEBullet_Header									// FRAME_INFO ** FmFrmInfo
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 8, Nme Pulsar
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move
		1000.0F,											// float	LifeCount
		{ 13, 16, 10 },										// uint16	FireDelay
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 30.0F, 31.0F, 32.0F },							// float	Damage
		{ PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS,									// float	Collision Radius
		  PULSAR_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize
		{ 108.0F, 180.0F, 255.0F },							// float	Light red value
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value
		FM_PULSAR,											// int16	FmSeq;
		&Pulsar_Header										// FRAME_INFO ** FmFrmInfo
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 9, Nme Trojax
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 0, 0, 0 },										// uint16	FireDelay         
		{ 1.0F, 2.0F, 3.0F },								// float	AmmoUsage         
		{ 64.0F, 160.0F, 256.0F },							// float	Damage            
		{ TROJAX_COLRADIUS,									// float	Collision Radius
		  TROJAX_COLRADIUS,									// float	Collision Radius
		  TROJAX_COLRADIUS },								// float	Collision Radius
		COLTYPE_Trojax,										// float	Collision Type
		( 2048.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 150.0F, 200.0F, 255.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value  
		FM_TROJAX,											// int16	FmSeq;            
		&Trojax1_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 10, Nme Pyrolite Rifle
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		64.0F,												// float	LifeCount         
		{ 2, 2, 2 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage         
		{ 2.0F, 4.0F, 6.0F },								// float	Damage            
		{ PYROLITE_COLRADIUS,								// float	Collision Radius
		  PYROLITE_COLRADIUS,								// float	Collision Radius
		  PYROLITE_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 128.0F, 128.0F, 128.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 16.0F, 16.0F, 16.0F },							// float	Light blue value  
		FM_FLAME,											// int16	FmSeq;            
		&Exp2_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 11, Nme Transpulse Cannon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 192.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 192.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 192.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 27, 21, 15 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage         
		{ 45.0F, 55.0F, 64.0F },							// float	Damage            
		{ TRANSPULSE_COLRADIUS,								// float	Collision Radius
		  TRANSPULSE_COLRADIUS,								// float	Collision Radius
		  TRANSPULSE_COLRADIUS },							// float	Collision Radius
		COLTYPE_Transpulse,									// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 0.0F, 0.0F, 0.0F },								// float	Light red value
		{ 108.0F, 180.0F, 255.0F },							// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value
		FM_ARC,												// int16	FmSeq;            
		&Arc_Header											// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 12, Nme Suss Gun
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 128.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 9, 7, 5 },										// uint16	FireDelay         
		{ 1.0F, 2.0F, 3.0F },								// float	AmmoUsage
		{ 11.0F, 13.0F, 15.0F },							// float	Damage            
		{ SUSSGUN_COLRADIUS,								// float	Collision Radius
		  SUSSGUN_COLRADIUS,								// float	Collision Radius
		  SUSSGUN_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 0.0F, 0.0F, 0.0F },								// float	Light red value 
		{ 84.0F, 168.0F, 255.0F },							// float	Light green value
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Flare_Header										// FRAME_INFO ** FmFrmInfo    
	},

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 13, Nme Laser
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		12.0F,												// float	LifeCount         
		{ 26, 22, 18 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 5.6F, 5.8F, 6.0F },								// float	Damage            
		{ LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 0.0F, 0.0F, 0.0F },								// float	Light red value 
		{ 255.0F, 255.0F, 255.0F },							// float	Light green value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Laser_Header										// FRAME_INFO ** FmFrmInfo    
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 14, Nme Lightning
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		12.0F,												// float	LifeCount         
		{ 26, 22, 18 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 5.6F, 5.8F, 6.0F },								// float	Damage            
		{ LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS,									// float	Collision Radius
		  LASER_COLRADIUS },								// float	Collision Radius
		COLTYPE_Point,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 255.0F, 255.0F, 255.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Laser_Header										// FRAME_INFO ** FmFrmInfo    
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 15, Flames
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{
		{( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ),							// float	Speed i can move  
		 ( 80.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		1000.0F,											// float	LifeCount         
		{ 2, 2, 2 },										// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage         
		{ 16.0F, 20.0F, 24.0F },							// float	Damage            
		{ FLAMES_COLRADIUS,									// float	Collision Radius
		  FLAMES_COLRADIUS,									// float	Collision Radius
		  FLAMES_COLRADIUS },								// float	Collision Radius
		COLTYPE_Sphere,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 128.0F, 128.0F, 128.0F },							// float	Light red value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light green value 
		{ 16.0F, 16.0F, 16.0F },							// float	Light blue value  
		FM_FLAME,											// int16	FmSeq;            
		&Exp2_Header										// FRAME_INFO ** FmFrmInfo    
	},
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Weapon 16, Nme Power Laser
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	{	
		{( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ),						// float	Speed i can move  
		 ( 32768.0F * GLOBAL_SCALE ) },						// float	Speed i can move  
		88.0F,												// float	LifeCount         
		{ 100, 100, 100 },									// uint16	FireDelay         
		{ 2.0F, 4.0F, 6.0F },								// float	AmmoUsage
		{ 8.0F, 9.5F, 10.0F },								// float	Damage
		{ PLASER_COLRADIUS,									// float	Collision Radius
		  PLASER_COLRADIUS,									// float	Collision Radius
		  PLASER_COLRADIUS },								// float	Collision Radius
		COLTYPE_Sphere,										// float	Collision Type
		( 1536.0F * GLOBAL_SCALE ),							// float	lightsize         
		{ 0.0F, 0.0F, 0.0F },								// float	Light red value 
		{ 255.0F, 255.0F, 255.0F },							// float	Light green value   
		{ 0.0F, 0.0F, 0.0F },								// float	Light blue value 
		FM_ARC2,											// int16	FmSeq;            
		&Laser_Header										// FRAME_INFO ** FmFrmInfo    
	},
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Set up And Init all PrimBulls
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	InitPrimBulls(void)
{
	uint16	i;
	int16	Count;
	FirstPrimBullUsed = (uint16) -1;
	FirstPrimBullFree = 0;
	for( i = 0 ; i < MAXPRIMARYWEAPONBULLETS ; i++ )
	{
		memset( &PrimBulls[i], 0, sizeof( PRIMARYWEAPONBULLET ) );
		PrimBulls[i].Used = FALSE;
		PrimBulls[i].Next = i + 1;
		PrimBulls[i].Prev = (uint16) -1;
		PrimBulls[i].Type = (uint16) -1;
		PrimBulls[i].Owner = (uint16) -1;
		PrimBulls[i].GroupImIn = (uint16) -1;
		PrimBulls[i].fmpoly = (uint16) -1;
		PrimBulls[i].light = (uint16) -1;
		PrimBulls[i].line = (uint16) -1;
		PrimBulls[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );
		if( AutoDetail )
		{
			if( avgframelag > 2.0F ) PrimBulls[i].TimeInterval +=  ( avgframelag - 1.0F );
		}
	}
	PrimBulls[MAXPRIMARYWEAPONBULLETS-1].Next = (uint16) -1;

	RestoreWeapons();
	RestoreAmmo();

	if( IsServerGame && IsServer )
	{
		for( Count = 0; Count < MAX_PLAYERS; Count++ )
		{
			HostRestoreWeapons( Count );
			HostRestoreAmmo( Count );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Restore ammo to starting amount
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RestoreWeapons( void )
{
	PrimaryWeaponsGot[ PULSAR ] = 1;
	PrimaryWeaponsGot[ TROJAX ] = 0;
	PrimaryWeaponsGot[ PYROLITE_RIFLE ] = 0;
	PrimaryWeaponsGot[ TRANSPULSE_CANNON ] = 0;
	PrimaryWeaponsGot[ SUSS_GUN ] = 0;
	PrimaryWeaponsGot[ LASER ] = 0;

	Ships[ WhoIAm ].Primary = PULSAR;
	Ships[ WhoIAm ].Secondary = MUGMISSILE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Restore ammo to starting amount
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void RestoreAmmo( void )
{
	int16	Count;

	PyroliteAmmo = 0.0F;				//MAXPYROLITEAMMO;
	SussGunAmmo = 0.0F;					//MAXSUSSGUNAMMO;
	GeneralAmmo = MAXGENERALAMMO;
	NitroFuel = 0.0F;

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecondaryAmmo[ Count ] = 0;
		SecondaryWeaponsGot[ Count ] = 0;
	}

	SecondaryWeaponsGot[ MUGMISSILE ] = 1;
	SecondaryAmmo[ MUGMISSILE ] = 5;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Restore ammo to starting amount
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HostRestoreWeapons( int16 Player )
{
	Host_PrimaryWeaponsGot[ Player ][ PULSAR ] = 1;
	Host_PrimaryWeaponsGot[ Player ][ TROJAX ] = 0;
	Host_PrimaryWeaponsGot[ Player ][ PYROLITE_RIFLE ] = 0;
	Host_PrimaryWeaponsGot[ Player ][ TRANSPULSE_CANNON ] = 0;
	Host_PrimaryWeaponsGot[ Player ][ SUSS_GUN ] = 0;
	Host_PrimaryWeaponsGot[ Player ][ LASER ] = 0;
	Host_PowerLevel[ Player ] = 0;
	Host_CopyOfPowerLevel[ Player ] = 0;
	Host_Flags[ Player ] = 0;

	Ships[ Player ].Primary = PULSAR;
	Ships[ Player ].Secondary = MUGMISSILE;
	Ships[ Player ].SuperNashramTimer = 0.0F;
	Ships[ Player ].InvulTimer = 0.0F;
	Ships[ Player ].StealthTime = 0.0F;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Restore ammo to starting amount
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HostRestoreAmmo( int16 Player )
{
	int16	Count;

	Host_PyroliteAmmo[ Player ] = 0.0F;				//MAXPYROLITEAMMO;
	Host_CopyOfPyroliteAmmo[ Player ] = 0.0F;		//MAXPYROLITEAMMO;
	Host_SussGunAmmo[ Player ] = 0.0F;				//MAXSUSSGUNAMMO;
	Host_CopyOfSussGunAmmo[ Player ] = 0.0F;		//MAXSUSSGUNAMMO;
	Host_GeneralAmmo[ Player ] = MAXGENERALAMMO;
	Host_CopyOfGeneralAmmo[ Player ] = MAXGENERALAMMO;
	Host_NitroFuel[ Player ] = 0.0F;
	Host_CopyOfNitroFuel[ Player ] = 0.0F;
	Host_NumOfOrbitals[ Player ] = 0;
	Host_CopyOfNumOfOrbitals[ Player ] = 0;

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		Host_SecondaryAmmo[ Player ][ Count ] = 0;
		Host_SecondaryWeaponsGot[ Player ][ Count ] = 0;
	}

	Host_SecondaryWeaponsGot[ Player ][ MUGMISSILE ] = 1;
	Host_SecondaryAmmo[ Player ][ MUGMISSILE ] = 5;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init ammo used to zero
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void InitAmmoUsed( void )
{
	int16	Count;
	int16	Player;

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecAmmoUsed[ Count ] = 0;
	}

	GeneralAmmoUsed = 0.0F;
	PyroliteAmmoUsed = 0.0F;
	SussGunAmmoUsed = 0.0F;
	NitroFuelUsed = 0.0F;

	if( IsServerGame && IsServer )
	{
		for( Player = 0; Player < MAX_PLAYERS; Player++ )
		{
			for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
			{
				Host_SecAmmoUsed[ Count ] = 0;
			}
		}
		Host_GeneralAmmoUsed = 0.0F;
		Host_PyroliteAmmoUsed = 0.0F;
		Host_SussGunAmmoUsed = 0.0F;
		Host_NitroFuelUsed = 0.0F;
		Host_BountyTimer = 0;
		Host_FlagTimer = 0;
		for ( Count = 0; Count < MAX_TEAMS; Count++ )
		{
			Host_TeamFlagTimer[ Count ] = 0;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Find a free PrimBull and move it from the free list to
					the used list
	Input		:	nothing
	Output		:	uint16 number of PrimBull free....
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16	FindFreePrimBull(void)
{
	uint16 i;

	i = FirstPrimBullFree;
	
	if ( i == (uint16) -1)
		return i;
 
	if( PrimBulls[i].Used )
	{
		// This Primary Bullet has been Used before....
		Msg( "%s Bullet has been Used more than once\n",DebugPrimStrings[PrimBulls[i].Type]  );
	}

	PrimBulls[i].Prev = FirstPrimBullUsed;
	if ( FirstPrimBullUsed != (uint16) -1)
	{
		PrimBulls[FirstPrimBullUsed].Next = i;
	}
	FirstPrimBullUsed = i;
	FirstPrimBullFree = PrimBulls[i].Next;

	PrimBulls[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );
	PrimBulls[i].TimeCount = 0.0F;

   	if( AutoDetail )
   	{
   		if( avgframelag > 2.0F ) PrimBulls[i].TimeInterval +=  ( avgframelag - 1.0F );
   	}
	PrimBulls[i].Used = TRUE;

	return i ;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill a used PrimBull and move it from the used list to
					the free list
	Input		:	uint16 number of PrimBull free....
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void	KillUsedPrimBull( uint16 i )
{
	uint16	its_prev;
	uint16	its_next;
	
	if( !PrimBulls[i].Used )
	{
		// This Primary Bullet has been Freed before....
		Msg( "%s Bullet has been Freed more than once\n",DebugPrimStrings[PrimBulls[i].Type]  );
	}

	its_prev = PrimBulls[i].Prev;
	its_next = PrimBulls[i].Next;

	if ( i == FirstPrimBullUsed )
		FirstPrimBullUsed = PrimBulls[i].Prev;

	if( its_prev != (uint16) -1)
		PrimBulls[its_prev].Next = its_next;

	if( its_next != (uint16) -1)
		PrimBulls[its_next].Prev = its_prev;

	PrimBulls[i].Prev = (uint16) -1;
	PrimBulls[i].Next = FirstPrimBullFree;
	FirstPrimBullFree	= i;
	PrimBulls[i].Used = FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if a new bullet is due....if it is init it
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float TrojaxFreqTable[] = {	0.6F, 0.8F, 1.0F };

float TrojaxChargeRateTable[] = { 1.0F, 1.5F, 2.0F };

#define	NO_AMMO_FIRERATE	120

void FirePrimary(void)
{
static int16	OnceOnlyFlag = 0;
	VECTOR	DownVector;
	VECTOR	LeftVector;
	VECTOR	Pos;
	VECTOR	Dir;
	VECTOR	UpVector;
	VECTOR	DirVector;
	VECTOR	Rotation;
	float	Ammo;
	int16	Count;
	float	Old_PowerLevel;
	float	Freq;
	uint16	Model;
	int		firing;
	float	fb_power;

	float	Speed;

	if( WhoIAm == MAX_PLAYERS ) return;

	if( GodModeOnceOnly )
	{
		if( !PlayDemo )
		{
			if( GodMode ) GivemeAllWeapons();
		}
		GodModeOnceOnly = FALSE;
	}

	firing = 0;
	fb_power = 0.0F;

	if( !GodMode )
	{
		if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
		{
			Ships[ WhoIAm ].SuperNashramTimer -= framelag;

			if( Ships[ WhoIAm ].SuperNashramTimer < 0.0F )
			{
				LoseSuperNashram();
			}
		}
	}

	if( control.drop_primary )
	{
		DropPrimary();
	}

	if( control.drop_shield )
	{
		DropShield();
	}

	if( control.drop_ammo )
	{
		DropAmmo();
	}

	PrimarySelectControl();

	Ships[ WhoIAm ].Object.Flags &= ~SHIP_Charging;

	// Bomb Mode...
	if( BombTag )
	{
		if( LowestBombTime == -1 )
		{
			return;
		}
	}
	
	if( control.select_primary )
	{
		if( PrimaryWeaponsGot[ control.select_primary - 1 ] )
		{
			switch( ( control.select_primary - 1 ) )
			{
				case PULSAR:
				case TROJAX:
				case TRANSPULSE_CANNON:
				case LASER:
					Ammo = GeneralAmmo;
					break;
				case PYROLITE_RIFLE:
					Ammo = PyroliteAmmo;
					break;
				case SUSS_GUN:
					Ammo = SussGunAmmo;
					break;

			}

			if( Ammo != 0 )
			{
				if( !OnceOnlyFlag )
				{

					OnceOnlyFlag = 1;

					if( PrimaryWeaponCheat )
					{
						if( Ships[ WhoIAm ].Primary == ( control.select_primary - 1 ) )
						{
							TogglePrimaryWeapons( Ships[ WhoIAm ].Primary );
						}
					}

					Ships[ WhoIAm ].Primary = ( control.select_primary - 1 );
					if( BikeCompSpeechSlider.value )
					{
						PlaySfx( PrimarySfxLookup[ Ships[ WhoIAm ].Primary ], 1.0F );
					}else
					{
						PlaySfx( SFX_WeaponSelect, 1.0F );
					}
				}
			}
			else
			{
				if( !OnceOnlyFlag )
				{
					OnceOnlyFlag = 1;
					if( !bSoundEnabled ) AddMessageToQue( YOU_DONT_HAVE_ANY_AMMO );
					PlaySfx( SFX_DontHaveThat, 1.0F );
				}
			}
		}
		else
		{
			if( !OnceOnlyFlag )
			{
				OnceOnlyFlag = 1;
				if( !bSoundEnabled ) AddMessageToQue( YOU_DONT_HAVE_THAT_WEAPON );
				PlaySfx( SFX_DontHaveThat, 1.0F );
			}
		}
	}
	else
	{
		OnceOnlyFlag = 0;
	}

	if( control.select_next_primary )
	{
		Ships[ WhoIAm ].Primary = GetNextValidPrimaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( PrimarySfxLookup[ Ships[ WhoIAm ].Primary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	if( control.select_prev_primary )
	{
		Ships[ WhoIAm ].Primary = GetPrevValidPrimaryWeapon();
		if( BikeCompSpeechSlider.value )
		{
			PlaySfx( PrimarySfxLookup[ Ships[ WhoIAm ].Primary ], 1.0F );
		}else
		{
			PlaySfx( SFX_WeaponSelect, 1.0F );
		}
	}

	if( Ships[ WhoIAm ].Primary == TROJAX )
	{
		if( control.fire_primary != 0 && GeneralAmmo )
		{
			if( PowerLevel > 10.0F ) Ships[ WhoIAm ].Object.Flags |= SHIP_Charging; // -- unpatched
			Ships[ WhoIAm ].Object.Flags |= SHIP_Charging;

			Old_PowerLevel = PowerLevel;

			PowerLevel += ( framelag * TrojaxChargeRateTable[ Ships[ WhoIAm ].Object.PowerLevel ] );

			if( PowerLevel > 100.0F )
			{
				PowerLevel = 100.0F;
				if ( TrojaxChargeID )
				{
					StopSfx( TrojaxChargeID ); 
					TrojaxChargeID = 0;
				}
			}

			if( ( Old_PowerLevel <= 10.0F ) && ( PowerLevel > 10.0F ) )
			{
				Freq = TrojaxFreqTable[ Ships[ WhoIAm ].Object.PowerLevel ];
				TrojaxChargeID = PlayPannedSfx( SFX_Trojax_Charge, Ships[ WhoIAm ].Object.Group , &Ships[ WhoIAm ].Object.Pos, Freq );
			}
		}
	}
	else
	{
		PowerLevel = 0.0F;
	}

	if( ( LaserTemperature && !control.fire_primary ) || ( LaserTemperature && LaserOverheated ) )
	{
		LaserTemperature -= ( framelag * 0.5F );
		if( LaserTemperature < 0.0F ) LaserTemperature = 0.0F;
		if( LaserTemperature < 30.0F ) LaserOverheated = 0;
	}

	if ( control.fire_primary && Ships[ WhoIAm ].Primary != TROJAX )
	{
		if ( ( Ships[ WhoIAm ].Primary == SUSS_GUN && SussGunAmmo > 0 ) ||
			 ( Ships[ WhoIAm ].Primary == PYROLITE_RIFLE && PyroliteAmmo > 0 ) ||
			 ( GeneralAmmo > 0 && ( Ships[ WhoIAm ].Primary != LASER || !LaserOverheated ) ) )
			 firing = 1;
	}

	if( ( Ships[ WhoIAm ].NumMultiples ) )
	{
		if( OrbitFireDelay <= 0.0F )
		{
			if( control.fire_primary )
			{
				OrbitFireDelay += (float) PrimaryWeaponAttribs[ ORBITPULSAR ].FireDelay[ Ships[ WhoIAm ].Object.PowerLevel ];
	
				for( Count = 0; Count < Ships[ WhoIAm ].NumMultiples; Count++ )
				{
					Model = Ships[ WhoIAm ].OrbModels[ Count ];

					if( Model != (uint16) -1 )
					{
						if( !GodMode && !( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
						{
							Ships[ WhoIAm ].OrbAmmo[ Count ] -= (float) PrimaryWeaponAttribs[ ORBITPULSAR ].AmmoUsage[ Ships[ WhoIAm ].Object.PowerLevel ];
						}

						if( Ships[ WhoIAm ].OrbAmmo[ Count ] < 0.0F )
						{
							if( Ships[ WhoIAm ].NumMultiples )
							{
								if( Ships[ WhoIAm ].OrbModels[ Ships[ WhoIAm ].NumMultiples - 1 ] != (uint16) -1 )
								{
									KillUsedModel( Ships[ WhoIAm ].OrbModels[ Ships[ WhoIAm ].NumMultiples - 1 ] );
									Ships[ WhoIAm ].OrbModels[ Ships[ WhoIAm ].NumMultiples - 1 ] = (uint16) -1;
								}

								Ships[ WhoIAm ].OrbAmmo[ Count ] = Ships[ WhoIAm ].OrbAmmo[ Ships[ WhoIAm ].NumMultiples - 1 ]; 

								NumOrbs++;
								Ships[ WhoIAm ].NumMultiples--;

								CreateOrbitPulsar( WhoIAm );
							}
						}
						else
						{
							Ships[ WhoIAm ].Object.Flags |= SHIP_MulFire;
							Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
							Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;
		
							Rotation.x = (float) sin( D2R( Models[ Model ].AxisRot ) );
							Rotation.y = (float) cos( D2R( Models[ Model ].AxisRot ) );
							Rotation.z = 0.0F;
							ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Rotation, &UpVector );	// Calc Up Vector
							ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Forward, &DirVector );	// Calc Dir Vector

							Pos.x = ( UpVector.x * ORBIT_RADIUS );
							Pos.y = ( UpVector.y * ORBIT_RADIUS );
							Pos.z = ( UpVector.z * ORBIT_RADIUS );
							InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, ORBITPULSAR,
											Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &DirVector, &UpVector,
											Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );


						}
					}
				}
			}
			else
			{
				if( OrbitFireDelay < 0.0F ) OrbitFireDelay = 0.0F;
			}
		}
		else
		{
			OrbitFireDelay -= framelag;
		}
	}

	if( PrimaryFireDelay <= 0.0F )
	{
		if( Ships[ WhoIAm ].Object.Mode == NORMAL_MODE )
		{
			if( !GeneralAmmo &&
				( ( !PyroliteAmmo && PrimaryWeaponsGot[ PYROLITE_RIFLE ] ) || ( !PrimaryWeaponsGot[ PYROLITE_RIFLE ] ) ) &&
				( ( !SussGunAmmo && PrimaryWeaponsGot[ SUSS_GUN ] ) || ( !PrimaryWeaponsGot[ SUSS_GUN ] ) ) )
			{
				if( control.fire_primary )
				{
					PrimaryFireDelay += NO_AMMO_FIRERATE;

					Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
					if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
					else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
					Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
					Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideDown, &DownVector );	/* Calc Up Vector */
	   				ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */

					if( Toggle )
					{
						Pos.x = ( LeftVector.x * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
						Pos.y = ( LeftVector.y * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
						Pos.z = ( LeftVector.z * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
						InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PULSAR,
										Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
										0, PowerLevel, FALSE );

					}
					if( !Toggle )
					{
						Pos.x = -( LeftVector.x * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
						Pos.y = -( LeftVector.y * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
						Pos.z = -( LeftVector.z * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
						InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PULSAR,
										Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
										0, PowerLevel, FALSE );
					}
					Toggle = !Toggle;
				}
			}
			else
			{
				if( control.fire_primary )
				{
					PrimaryFireDelay += (float) PrimaryWeaponAttribs[ PrimaryToFireLookup[ Ships[WhoIAm].Primary ] ].FireDelay[ Ships[ WhoIAm ].Object.PowerLevel ];
		
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */
					ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideDown, &DownVector );	/* Calc Up Vector */
	   				ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */
		
					if( PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] )
					{
						int8 prm;
						if(LightningLaser)
							prm = PrimaryToFireLookup2[ Ships[WhoIAm].Primary ];
						else
							prm = PrimaryToFireLookup[ Ships[WhoIAm].Primary ];

						switch( prm )
						{
							case PULSAR:
								if( GeneralAmmo )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									if( Ships[ WhoIAm ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;

									if( ( Toggle ) || ( Ships[ WhoIAm ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) )
									{
										Pos.x = ( LeftVector.x * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
										Pos.y = ( LeftVector.y * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
										Pos.z = ( LeftVector.z * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
														Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
														Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

									}

									if( Ships[ WhoIAm ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;

									if( ( !Toggle ) || ( Ships[ WhoIAm ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) )
									{
										Pos.x = -( LeftVector.x * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
										Pos.y = -( LeftVector.y * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
										Pos.z = -( LeftVector.z * ( 100.0F * GLOBAL_SCALE ) ) + ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
														Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
														Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
									Toggle = !Toggle;
								}
								break;
			
							case TROJAX:								/* Fire on release or when max reached */
								if( GeneralAmmo )
								{
									if( !GodMode && !( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
									{
										Ammo = PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].AmmoUsage[ Ships[ WhoIAm ].Object.PowerLevel ] * framelag;
										GeneralAmmoUsed += Ammo;
										GeneralAmmo -= Ammo;
									}
									if( GeneralAmmo <= 0.0F )
									{
										GeneralAmmo = 0.0F;
										if( PowerLevel > 10.0F )
										{
											VECTOR recoil;

											if( TrojaxChargeID )
											{
												StopSfx( TrojaxChargeID ); 
											}

											Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
											if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
											else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
											Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
											Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

											ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
											ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideDown, &DownVector );		/* Calc Up Vector */
							   				ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */
						
											Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
											Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
											Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
											InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
															Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
															Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
			
											firing = 1;

											recoil.x = 0.0F;
											recoil.y = 0.0F;
											recoil.z = -( ( PowerLevel / 5.0F ) * GLOBAL_SCALE );
											ForceInternalOneOff( WhoIAm, &recoil );
			
											PowerLevel = 0.0F;
										}
									}
								}
								break;
			
							case PYROLITE_RIFLE:
								if( PyroliteAmmo )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									PyroLightFlag = TRUE;

									Pos.x = ( DownVector.x * ( 100.0F * GLOBAL_SCALE ) );
									Pos.y = ( DownVector.y * ( 100.0F * GLOBAL_SCALE ) );
									Pos.z = ( DownVector.z * ( 100.0F * GLOBAL_SCALE ) );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

#if 1
									Speed = (float) PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].FireDelay[ Ships[ WhoIAm ].Object.PowerLevel ] * framelag;
									Speed *= ( ( PrimaryWeaponAttribs[ Ships[ WhoIAm ].Primary ].Speed[ Ships[ WhoIAm ].Object.PowerLevel ] ) * 0.333333F );
									Pos.x += ( Dir.x * Speed );
									Pos.y += ( Dir.y * Speed );
									Pos.z += ( Dir.z * Speed );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

									Pos.x += ( Dir.x * Speed );
									Pos.y += ( Dir.y * Speed );
									Pos.z += ( Dir.z * Speed );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

#endif
								}
								break;
			
							case TRANSPULSE_CANNON:

								if( GeneralAmmo )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
									Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
									Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

								}

								break;
			
							case SUSS_GUN:
								if( SussGunAmmo )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;
								}

								for( Count = 0; Count < 4; Count++ )
								{
									Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
									Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
									Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );

									if( SussGunAmmo )
									{
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
														Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
														Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
								}
								break;
			
							case LASER:

								if( GeneralAmmo && !LaserOverheated )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									Pos.x = ( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									Pos.y = (    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
									Pos.z = (    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary, 
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

//								}
//								if( GeneralAmmo && !LaserOverheated )
//								{
									Pos.x = -( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									Pos.y = -(    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
									Pos.z = -(    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary, 
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );


								}

								break;

							case NME_LIGHTNING:

								if( GeneralAmmo && !LaserOverheated )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									Pos.x = ( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									Pos.y = (    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
									Pos.z = (    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									if(LightningLaser)
									{
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PrimaryToFireLookup2[ Ships[WhoIAm].Primary ],
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
									else
									{
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PrimaryToFireLookup[ Ships[WhoIAm].Primary ],
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
								}
								if( GeneralAmmo && !LaserOverheated )
								{
									Pos.x = -( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									Pos.y = -(    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
									Pos.z = -(    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
									if(LightningLaser)
									{
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PrimaryToFireLookup2[ Ships[WhoIAm].Primary ],
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
									else
									{
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PrimaryToFireLookup[ Ships[WhoIAm].Primary ],
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );
									}
								}

								break;

							case NME_POWERLASER:

								if( GeneralAmmo )
								{
									Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
									if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
									else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
									Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
									Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

									Pos.x = (   0.0F * GLOBAL_SCALE );
									Pos.y = ( -75.0F * GLOBAL_SCALE );
									Pos.z = (   0.0F * GLOBAL_SCALE );
									InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, PrimaryToFireLookup[ Ships[WhoIAm].Primary ],
													Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
													Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

								}

								break;


							default:
								break;
						}
					}
				}
				else
				{
					if( PrimaryFireDelay < 0.0F ) PrimaryFireDelay = 0.0F;
					
					if( PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] )
					{
						switch( Ships[WhoIAm].Primary )
						{
							case TROJAX:
								if( GeneralAmmo )
								{
									if( PowerLevel > 10.0F )
									{
										VECTOR recoil;
		 
										if( TrojaxChargeID ) 
										{
											StopSfx( TrojaxChargeID ); 
										}

										Ships[ WhoIAm ].Object.Flags |= SHIP_PrimFire;
										if( Toggle ) Ships[ WhoIAm ].Object.Flags |= SHIP_PrimToggle;
										else Ships[ WhoIAm ].Object.Flags &= ~SHIP_PrimToggle;
										Ships[ WhoIAm ].PrimPowerLevel = PowerLevel;
										Ships[ WhoIAm ].PrimID = Ships[ WhoIAm ].PrimBullIdCount + 1;

										ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &Dir );
										ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideDown, &DownVector );		/* Calc Up Vector */
						   				ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */
					
										Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
										Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
										Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
										InitOnePrimBull( OWNER_SHIP, WhoIAm, ++Ships[ WhoIAm ].PrimBullIdCount, Ships[ WhoIAm ].Primary,
														Ships[ WhoIAm ].Object.Group, &Ships[ WhoIAm ].Object.Pos, &Pos, &Dir, &UpVector,
														Ships[ WhoIAm ].Object.PowerLevel, PowerLevel, FALSE );

										firing = 1;
										fb_power = PowerLevel / MAX_TROJAX_POWER_LEVEL;

										recoil.x = 0.0F;
										recoil.y = 0.0F;
										recoil.z = -( ( PowerLevel / 5.0F ) * GLOBAL_SCALE );
										ForceInternalOneOff( WhoIAm, &recoil );
		
										PowerLevel = 0.0F;
									}
								}
								break;
			
							case PULSAR:
							case PYROLITE_RIFLE:
							case TRANSPULSE_CANNON:
							case SUSS_GUN:
							case LASER:
							case NME_POWERLASER:
							case NME_LIGHTNING:
							default:
								break;
						}
					}
				}
			}
		}
	}
	else
	{
		PrimaryFireDelay -= framelag;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Process Primary Bullets
	Input		:	nothing
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
static float PyroCount = 0.0F;

void ProcessPrimaryBullets( void )
{
	uint16			i;
	uint16			Count;
	uint16			nextprim;
	VECTOR			TempVector;
	float			Speed;
	VECTOR			temp;
	VECTOR			NewDir;
	uint16			fmpoly;
	uint16			nextfmpoly;
	uint16			poly;
	uint16			nextpoly;
	float			Damage;
	VECTOR			DirVector;
	VECTOR			NewPos;
	VECTOR			Int_Point;
	VECTOR			Int_Point2;
	uint16			HitTarget;
	uint16			HitWall;
	float			DistFromStart;
	float			Length;
	uint16			HitSecondary;
	VECTOR			MInt_Point;
	VECTOR			MInt_Point2;
	float			DistToInt;
	VECTOR			Recoil;
	VECTOR			MoveOffsetVector;
	float			DistToCenter;
	BGOBJECT	*	BGObject;
	VERT			BGPoint;
	NORMAL			BGNormal;
	float			BGDamage;
	ENEMY		*	HitEnemy;
	VECTOR			EInt_Point;
	VECTOR			EInt_Point2;
	NORMAL			ENormal;
	uint16			TempEnemyIndex;
	VECTOR			TempNormal = { 0.0F , 1.0F , 0.0F };
	GUNOBJECT	*	GunPtr;		// the first gun...
	FIREPOS		*	FirePosPnt;
	BOOL			Killed;
	PVSPOTFX	*	SpotFXPtr;
	VECTOR			TrigPos;
	float			NewFramelag = 0.0F;

	PyroCount += framelag;

	i = FirstPrimBullUsed;
	while( i != (uint16) -1 )
	{
		nextprim = PrimBulls[i].Prev;

		NewFramelag = ( framelag + PrimBulls[i].FramelagAddition );
		if( PrimBulls[i].FramelagAddition != 0.0F )
		{
			PrimBulls[i].FramelagAddition = 0.0F;
		}

		if (PrimBulls[i].LifeCount > 0.0F)
		{
			PrimBulls[i].LifeCount -= NewFramelag; //framelag;
			if( PrimBulls[i].LifeCount < 0.0F ) PrimBulls[i].LifeCount = 0.0F;

			switch( PrimBulls[i].Weapon )
			{
				case LASER:
				case NME_LASER:
				case NME_LIGHTNING:
				case NME_POWERLASER:
					switch( PrimBulls[i].OwnerType )
					{
						case OWNER_SHIP:
				   			ApplyMatrix( &Ships[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].Offset, &TempVector );
							PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, &Ships[ PrimBulls[i].Owner ].Object.Pos, Ships[ PrimBulls[i].Owner ].Object.Group, &TempVector );
							ApplyMatrix( &Ships[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].Offset, &TempVector );
							ApplyMatrix( &Ships[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].LocalDir, &PrimBulls[i].Dir );
							PrimBulls[i].Pos.x = ( Ships[ PrimBulls[i].Owner ].Object.Pos.x + TempVector.x );
							PrimBulls[i].Pos.y = ( Ships[ PrimBulls[i].Owner ].Object.Pos.y + TempVector.y );
							PrimBulls[i].Pos.z = ( Ships[ PrimBulls[i].Owner ].Object.Pos.z + TempVector.z );
							PrimBulls[i].ColFlag = 0;
							break;

						case OWNER_ENEMY:
							if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
							{
					   			ApplyMatrix( &Enemies[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].Offset, &TempVector );
								PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, &Enemies[ PrimBulls[i].Owner ].Object.Pos, Enemies[ PrimBulls[i].Owner ].Object.Group, &TempVector );
								ApplyMatrix( &Enemies[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].Offset, &TempVector );
								ApplyMatrix( &Enemies[ PrimBulls[i].Owner ].Object.FinalMat, &PrimBulls[i].LocalDir, &PrimBulls[i].Dir );
								PrimBulls[i].Pos.x = ( Enemies[ PrimBulls[i].Owner ].Object.Pos.x + TempVector.x );
								PrimBulls[i].Pos.y = ( Enemies[ PrimBulls[i].Owner ].Object.Pos.y + TempVector.y );
								PrimBulls[i].Pos.z = ( Enemies[ PrimBulls[i].Owner ].Object.Pos.z + TempVector.z );
								PrimBulls[i].ColFlag = 0;
							}
							else
							{
								GunPtr = PrimBulls[i].EnemyGun;
								if( GunPtr )
								{
									FirePosPnt = EnemyTypes[ Enemies[ PrimBulls[i].Owner ].Type ].GunFirePoints[ GunPtr->GunNum>>1 ];
									FirePosPnt += PrimBulls[i].FirePoint;
									ApplyMatrix( &GunPtr->Mat , &FirePosPnt->Points[0] , &TempVector );
									TempVector.x += GunPtr->FirePos.x;
									TempVector.y += GunPtr->FirePos.y;
									TempVector.z += GunPtr->FirePos.z;
									PrimBulls[i].Pos = TempVector;
									TempVector.x -= Enemies[ PrimBulls[i].Owner ].Object.Pos.x;
									TempVector.y -= Enemies[ PrimBulls[i].Owner ].Object.Pos.y;
									TempVector.z -= Enemies[ PrimBulls[i].Owner ].Object.Pos.z;
									PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, &Enemies[ PrimBulls[i].Owner ].Object.Pos, Enemies[ PrimBulls[i].Owner ].Object.Group, &TempVector );
									ApplyMatrix( &GunPtr->Mat, &PrimBulls[i].LocalDir, &PrimBulls[i].Dir );
									PrimBulls[i].ColFlag = 0;
								}
							}
							break;

						case OWNER_NOBODY:
							PrimBulls[i].ColFlag = 0;
							break;

						case OWNER_MODELSPOTFX:
							if( ModelNames[ Models[ PrimBulls[i].Owner ].ModelNum ].DoIMorph )
							{
								SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[i].Owner ].ModelNum ].SpotFX + PrimBulls[i].SpotFX );
							}
							else
							{
								SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[i].Owner ].ModelNum ].SpotFX + PrimBulls[i].SpotFX );
							}

							ApplyMatrix( &Models[ PrimBulls[i].Owner ].Mat, &SpotFXPtr->Pos, &TempVector );
							PrimBulls[i].Pos.x = ( Models[ PrimBulls[i].Owner ].Pos.x + TempVector.x );
							PrimBulls[i].Pos.y = ( Models[ PrimBulls[i].Owner ].Pos.y + TempVector.y );
							PrimBulls[i].Pos.z = ( Models[ PrimBulls[i].Owner ].Pos.z + TempVector.z );
							PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, &Models[ PrimBulls[i].Owner ].Pos, Models[ PrimBulls[i].Owner ].Group, &TempVector );
							ApplyMatrix( &Models[ PrimBulls[i].Owner ].Mat, &SpotFXPtr->DirVector, &PrimBulls[i].Dir );
							PrimBulls[i].ColFlag = 0;
							break;

						default:
							break;

					}
	 				break;

   	   			case TRANSPULSE_CANNON:
				case NME_TRANSPULSE:
					if( IsGroupVisible[ PrimBulls[i].GroupImIn ] )
					{
						PrimBulls[i].TimeCount += NewFramelag; //framelag;
		
						if( PrimBulls[i].TimeCount >= PrimBulls[i].TimeInterval )
						{
							PrimBulls[i].TimeCount = 0.0F;
							CreateTransTrail( i );
						}
					}
					else
					{
						PrimBulls[i].TimeCount = 0.0F;
					}
					break;

   	   			case PYROLITE_RIFLE:
				case NME_PYROLITE:
					fmpoly = PrimBulls[i].fmpoly;
					if( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].xsize += ( NewFramelag * ( 0.075F * GLOBAL_SCALE ) ); //framelag
						FmPolys[ fmpoly ].ysize += ( NewFramelag * ( 0.075F * GLOBAL_SCALE ) );	//framelag
					}
					break;

				case FLAMES:
   	   			case PULSAR:
   	   			case TROJAX:
   	   			case SUSS_GUN:
   	   			case ORBITPULSAR:
   	   			case NME_BULLET1:
   	   			case NME_PULSAR:
   	   			case NME_TROJAX:
   	   			case NME_SUSS_GUN:
   	   			default:
   	   				break;
			}

			if( AmIOutsideGroup( &Mloadheader, &PrimBulls[i].Pos, PrimBulls[i].GroupImIn ) )
			{
				DebugPrintf( "Killed %s for getting out of group %s\n", DebugPrimStrings[ PrimBulls[i].Weapon ],
							 (SecBulls[i].GroupImIn == (uint16) -1) ? "(outside)" : Mloadheader.Group[ SecBulls[i].GroupImIn ].name );
				CleanUpPrimBull( i, TRUE );
				goto loop;
			}

			Speed = ( PrimBulls[ i ].Speed * NewFramelag ); //framelag );
			NewPos.x = PrimBulls[ i ].Pos.x + ( PrimBulls[ i ].Dir.x * Speed );
			NewPos.y = PrimBulls[ i ].Pos.y + ( PrimBulls[ i ].Dir.y * Speed );
			NewPos.z = PrimBulls[ i ].Pos.z + ( PrimBulls[ i ].Dir.z * Speed );

			DirVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
			DirVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
			DirVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

			MoveOffsetVector = DirVector;

			Length = VectorLength( &DirVector );

			NormaliseVector( &DirVector );

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Do Collision with background when initialised
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( !PrimBulls[i].ColFlag )
			{
				temp.x = ( PrimBulls[i].Dir.x * MaxColDistance);
				temp.y = ( PrimBulls[i].Dir.y * MaxColDistance);
				temp.z = ( PrimBulls[i].Dir.z * MaxColDistance);

				if( !BackgroundCollide( &MCloadheadert0, &Mloadheader, &PrimBulls[i].Pos,
										PrimBulls[i].GroupImIn, &temp, (VECTOR *) &PrimBulls[i].ColPoint,
										&PrimBulls[i].ColGroup, &PrimBulls[i].ColPointNormal, &TempVector, FALSE, NULL ) )
				{
					DebugPrintf( "Primary weapon %d didn't collide with backgroup in group %d\n", PrimBulls[i].Weapon, PrimBulls[i].GroupImIn );
					if( DebugInfo ) CreateDebugLine( &PrimBulls[i].Pos, &temp, PrimBulls[i].GroupImIn, 255, 64, 64 );
					CleanUpPrimBull( i, TRUE );
					goto loop;
				}
				else PrimBulls[i].ColFlag = 1;

				PrimBulls[i].ColStart = PrimBulls[i].Pos;
				PrimBulls[i].ColDist = (float) fabs( DistanceVert2Vector( (VERT*) &PrimBulls[i].ColPoint, &PrimBulls[i].Pos ) );
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			DistFromStart = (float) fabs( DistanceVert2Vector( (VERT*) &NewPos, &PrimBulls[i].ColStart ) );

			HitWall = (uint16) -1;
			DistToInt = 100000.0F;									/* Distance to Intersection */

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( PrimBulls[i].ColDist < DistFromStart )
		   	{
				HitWall = 0;
				DistToInt = (float) fabs( DistanceVert2Vector( (VERT*) &PrimBulls[i].ColPoint, &PrimBulls[i].Pos ) );

				if( OneGroupBGObjectCol( DistToInt, 1, PrimBulls[i].GroupImIn, &PrimBulls[i].Pos, &MoveOffsetVector,
										 (VECTOR *) &BGPoint, &BGNormal, &TempVector, &BGObject, 0.0F ) )
				{
					DistToInt = (float) fabs( DistanceVert2Vector( &BGPoint, &PrimBulls[i].Pos ) );
					PrimBulls[i].ColPoint = BGPoint;
					PrimBulls[i].ColPointNormal = BGNormal;
					if( BGObject ) PrimBulls[i].ColGroup = BGObject->Group;
					BGDamage = PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];
					if( ( PrimBulls[ i ].Weapon == LASER ) ||
						( PrimBulls[ i ].Weapon == NME_LASER ) ||
						( PrimBulls[ i ].Weapon == NME_POWERLASER ) )
					{
						BGDamage *= ( framelag / 3.0F );
					}
					if( BGObject ) ChangeBGState( BGObject, PrimBulls[i].OwnerType, PrimBulls[i].Owner, SHOT, BGDamage );
				}
   			}
			else
			{
				if( OneGroupBGObjectCol( 0.0F, 0, PrimBulls[i].GroupImIn, &PrimBulls[i].Pos, &MoveOffsetVector,
										 (VECTOR *) &BGPoint, &BGNormal, &TempVector, &BGObject, 0.0F ) )
				{
					DistToInt = (float) fabs( DistanceVert2Vector( &BGPoint, &PrimBulls[i].Pos ) );
					PrimBulls[i].ColPoint = BGPoint;
					PrimBulls[i].ColPointNormal = BGNormal;
					if( BGObject ) PrimBulls[i].ColGroup = BGObject->Group;
					HitWall = 0;
					BGDamage = PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];
					if( ( PrimBulls[ i ].Weapon == LASER ) ||
						( PrimBulls[ i ].Weapon == NME_LASER ) ||
						( PrimBulls[ i ].Weapon == NME_POWERLASER ) )
					{
						BGDamage *= ( framelag / 3.0F );
					}
					if( BGObject ) ChangeBGState( BGObject, PrimBulls[i].OwnerType, PrimBulls[i].Owner, SHOT, BGDamage );
				}
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			Damage = (int16) PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];

			if ( WaterObjectCollide( PrimBulls[i].GroupImIn,&PrimBulls[i].Pos, &MoveOffsetVector, &Int_Point , Damage) )
			{
				CreateSplash( (VECTOR *) &Int_Point, &PrimBulls[i].Dir, PrimBulls[i].GroupImIn );
			}
			
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			HitTarget = CheckHitShip( PrimBulls[i].OwnerType, PrimBulls[i].Owner, &PrimBulls[i].Pos, PrimBulls[i].GroupImIn, &DirVector, &PrimBulls[i].UpVector, Length, &Int_Point, &Int_Point2, &DistToInt, PrimBulls[i].ColRadius, PrimBulls[i].ColType );
			if( HitTarget != (uint16) -1 ) HitWall = (uint16) -1;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			HitSecondary = CheckHitSecondary( &PrimBulls[i].Pos, PrimBulls[i].GroupImIn, &DirVector, &PrimBulls[i].UpVector, Length, &MInt_Point, &MInt_Point2, &DistToInt, PrimBulls[i].ColRadius, PrimBulls[i].ColType );
			if( HitSecondary != (uint16) -1 )
			{
				HitWall = (uint16) -1;
				HitTarget = (uint16) -1;
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			HitEnemy = CheckHitEnemy( PrimBulls[i].OwnerType, PrimBulls[i].Owner, &PrimBulls[i].Pos, &DirVector, &PrimBulls[i].UpVector, Length, &EInt_Point, &EInt_Point2, &DistToInt, PrimBulls[i].ColRadius, PrimBulls[i].ColType );
			if( HitEnemy != NULL )
			{
				HitWall = (uint16) -1;
				HitTarget = (uint16) -1;
				HitSecondary = (uint16) -1;
			}
			else
			{
				TempEnemyIndex = GetComEnemyHitInfo( &EInt_Point, &ENormal, &DistToInt );

				if( TempEnemyIndex != (uint16) -1 )
				{
					HitEnemy = &Enemies[ TempEnemyIndex ];
					EInt_Point2 = EInt_Point;
					HitWall = (uint16) -1;
					HitTarget = (uint16) -1;
					HitSecondary = (uint16) -1;
				}
			}

			if( ( HitWall != (uint16) -1 ) || ( HitTarget != (uint16) -1 ) || ( HitSecondary != (uint16) -1 ) || ( HitEnemy != NULL ) )
			{
				TrigPos.x = PrimBulls[i].Pos.x + ( PrimBulls[i].Dir.x * DistToInt );
				TrigPos.y = PrimBulls[i].Pos.y + ( PrimBulls[i].Dir.y * DistToInt );
				TrigPos.z = PrimBulls[i].Pos.z + ( PrimBulls[i].Dir.z * DistToInt );
			}
			else
			{
				TrigPos = NewPos;
			}

			switch( PrimBulls[i].OwnerType )
			{
				case OWNER_SHIP:
					TriggerAreaPlayerShootsCheck( &PrimBulls[i].Pos, &TrigPos, PrimBulls[i].GroupImIn, WEPTYPE_Primary, PrimBulls[i].Weapon );
					break;

				case OWNER_ENEMY:
					TriggerAreaEnemyShootsCheck( &PrimBulls[i].Pos, &TrigPos, PrimBulls[i].GroupImIn, WEPTYPE_Primary, PrimBulls[i].Weapon );
					break;
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Secondary Weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( HitSecondary != (uint16) -1 )
			{
				TempVector.x = ( MInt_Point.x - SecBulls[ HitSecondary ].Pos.x );
				TempVector.y = ( MInt_Point.y - SecBulls[ HitSecondary ].Pos.y );
				TempVector.z = ( MInt_Point.z - SecBulls[ HitSecondary ].Pos.z );
				DistToCenter = VectorLength( &TempVector );
				NormaliseVector( &TempVector );

		  		ReflectVector( &PrimBulls[i].Dir, (NORMAL *) &TempVector, &NewDir );

				Damage = PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];

				switch( PrimBulls[ i ].Weapon )
				{
					case PULSAR:
	   	   			case NME_BULLET1:
	   	   			case NME_PULSAR:
	   	   			case NME_TROJAX:
	   	   			case NME_PYROLITE:
	   	   			case NME_SUSS_GUN:
					case TROJAX:
					case PYROLITE_RIFLE:
					case SUSS_GUN:
					case ORBITPULSAR:
					case FLAMES:
						break;

					case TRANSPULSE_CANNON:
	   	   			case NME_TRANSPULSE:
						if( !PrimBulls[i].Bounces ) // Damage /= 4.0F; // -- patched
						Damage /= ( ( TRANSPULSE_BOUNCES + 1 ) - PrimBulls[i].Bounces ); // - unpatched
						break;

					case LASER:
	   	   			case NME_LASER:
	   	   			case NME_LIGHTNING:
					case NME_POWERLASER:
						TempVector.x = ( MInt_Point.x - PrimBulls[ i ].Pos.x );
						TempVector.y = ( MInt_Point.y - PrimBulls[ i ].Pos.y );
						TempVector.z = ( MInt_Point.z - PrimBulls[ i ].Pos.z );
						DistToInt = VectorLength( &TempVector );
						if( DistToInt > LASER_MINDAMAGERANGE ) DistToInt = LASER_MINDAMAGERANGE;
						Damage = ( Damage * ( 1.25F - ( ( DistToInt / LASER_MINDAMAGERANGE ) / 2.0F ) ) );
						Damage = ( Damage * ( framelag / 3.0F ) );
						break;
				}

				if( DistToCenter >= ( SHIP_RADIUS + 1.0F ) )
				{
					Damage =  ( Damage / 10.0F ) * framelag;
				}

				SecBulls[ HitSecondary ].Shield -= Damage;
				if( SecBulls[ HitSecondary ].Shield < 0.0F ) SecBulls[ HitSecondary ].Shield = 0.0F;
				if( SecBulls[ HitSecondary ].Shield == 0.0F )
				{
					switch( SecBulls[ HitSecondary ].Weapon )
					{
						case ENEMYSPIRALMISSILE:
						case ENEMYHOMINGMISSILE:
						case ENEMYBLUEHOMINGMISSILE:
						case ENEMYDEPTHCHARGE:
						case GRAVGONMISSILE:
						case MUGMISSILE:
						case MULTIPLEMISSILE:
						case SOLARISMISSILE:
						case SCATTERMISSILE:
						case ENEMYTENTACLE:
						default:
							MissileShockWave( (VECTOR *) &MInt_Point, MISSILE_SHOCKWAVE, SecBulls[ HitSecondary ].Owner,
											( SecBulls[ HitSecondary ].Damage / 2.5F ), SecBulls[ HitSecondary ].GroupImIn,
											SecBulls[ HitSecondary ].Weapon );
							DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
							break;

						case ENEMYFIREBALL:
							DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
							break;

						case THIEFMISSILE:
							DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
							break;

						case PURGEMINE:
						case PINEMINE:
						case QUANTUMMINE:
						case SPIDERMINE:
						case PINEMISSILE:
						case TITANSTARSHRAPNEL:
							DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
							break;

						case TITANSTARMISSILE:
							TempVector.x = 0.0F;
							TempVector.y = 1.0F;
							TempVector.z = 0.0F;
							CreateExplosionDonut( &SecBulls[ HitSecondary ].Pos, (VECTOR *) &TempVector, SecBulls[ HitSecondary ].GroupImIn );
							if( ( ( SecBulls[ HitSecondary ].OwnerType == OWNER_SHIP ) && ( SecBulls[ HitSecondary ].Owner == WhoIAm ) ) ||
								( ( SecBulls[ HitSecondary ].OwnerType == OWNER_ENEMY ) && ( IsHost ) ) ||
								( ( SecBulls[ HitSecondary ].OwnerType == OWNER_NOBODY ) && ( IsHost ) ) )
							{
								CreateTitanStarShrapnel( HitSecondary, (VECTOR *) &MInt_Point, &SecBulls[ HitSecondary ].DirVector, SecBulls[ HitSecondary ].GroupImIn );
							}
							DestroySecondary( HitSecondary, (VECTOR *) &MInt_Point );
							break;
					}
				}

				Killed = FALSE;

				switch( PrimBulls[ i ].Weapon )
				{
	   	   			case NME_BULLET1:
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

	   	   			case NME_PULSAR:
						CreateNmePulsarExplosion( &MInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case PULSAR:
						CreatePulsarExplosion( &MInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case NME_PYROLITE:
					case PYROLITE_RIFLE:
						NewPos.x = MInt_Point.x;
						NewPos.y = MInt_Point.y;
						NewPos.z = MInt_Point.z;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

						PrimBulls[i].Dir = NewDir;
						PrimBulls[i].ColFlag = 0;
						break;

					case FLAMES:
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case NME_TROJAX:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateNmeTrojaxExplosion( &MInt_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;
					case TROJAX:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateTrojaxExplosion( &MInt_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case NME_TRANSPULSE:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateNmeArcExplosion( &MInt_Point, &TempVector, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;
					case TRANSPULSE_CANNON:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateArcExplosion( &MInt_Point, &TempVector, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case NME_SUSS_GUN:
						CreateNmeShrapnelExplosion( &MInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case SUSS_GUN:
						CreateShrapnelExplosion( &MInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

    				case LASER:
						CreateLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case NME_LASER:
						CreateNmeLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case NME_POWERLASER:
						CreateNmePowerLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case NME_LIGHTNING:
						CreateNmeLightningPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case ORBITPULSAR:
						CreatePulsarExplosion( &MInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;
					default:
						break;
				}

				if( Killed )
				{
					goto loop;
				}
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Background or Background Object
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( HitWall != (uint16) -1 )
			{
		  		ReflectVector( &PrimBulls[i].Dir, &PrimBulls[i].ColPointNormal, &NewDir );

				Killed = FALSE;

   				switch( PrimBulls[i].Weapon )
   				{
					case NME_BULLET1:
						CreateSparks( (VECTOR *) &PrimBulls[i].ColPoint, (VECTOR *) &PrimBulls[i].ColPointNormal, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

   					case NME_PULSAR:
						CreateNmePulsarExplosion( (VECTOR *) &PrimBulls[i].ColPoint, &NewDir, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case PULSAR:
						CreatePulsarExplosion( (VECTOR *) &PrimBulls[i].ColPoint, &NewDir, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case NME_PYROLITE:
   					case PYROLITE_RIFLE:
						NewPos.x = PrimBulls[i].ColPoint.x;
						NewPos.y = PrimBulls[i].ColPoint.y;
						NewPos.z = PrimBulls[i].ColPoint.z;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						PrimBulls[i].Dir = NewDir;
						PrimBulls[i].ColFlag = 0;
   						break;

					case FLAMES:
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

   					case NME_TROJAX:
   						CreateNmeTrojaxExplosion( (VECTOR *) &PrimBulls[i].ColPoint, (VECTOR *) &PrimBulls[i].ColPointNormal, PrimBulls[i].fmpoly, 0, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case TROJAX:
   						CreateTrojaxExplosion( (VECTOR *) &PrimBulls[i].ColPoint, (VECTOR *) &PrimBulls[i].ColPointNormal, PrimBulls[i].fmpoly, 0, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case NME_TRANSPULSE:
   						CreateNmeArcExplosion( (VECTOR *) &PrimBulls[i].ColPoint, (VECTOR *) &PrimBulls[i].ColPointNormal, PrimBulls[i].GroupImIn );

						NewPos.x = PrimBulls[i].ColPoint.x;
						NewPos.y = PrimBulls[i].ColPoint.y;
						NewPos.z = PrimBulls[i].ColPoint.z;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

						if( !ChangeTranspulseDir( i, &NewPos, &MoveOffsetVector, &NewDir ) )
						{
	   						CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
   						break;

   					case TRANSPULSE_CANNON:
   						CreateArcExplosion( (VECTOR *) &PrimBulls[i].ColPoint, (VECTOR *) &PrimBulls[i].ColPointNormal, PrimBulls[i].GroupImIn );

						NewPos.x = PrimBulls[i].ColPoint.x;
						NewPos.y = PrimBulls[i].ColPoint.y;
						NewPos.z = PrimBulls[i].ColPoint.z;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

						if( !ChangeTranspulseDir( i, &NewPos, &MoveOffsetVector, &NewDir ) )
						{
	   						CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
   						break;

   					case NME_SUSS_GUN:
   						CreateNmeShrapnelExplosion( (VECTOR *) &PrimBulls[i].ColPoint, &NewDir, PrimBulls[i].GroupImIn );
						if( !Random_Range( 16 ) )
						{
							PlayPannedSfx( SFX_Ric , PrimBulls[i].ColGroup , (VECTOR *) &PrimBulls[i].ColPoint, 0.0F );
						}
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case SUSS_GUN:
   						CreateShrapnelExplosion( (VECTOR *) &PrimBulls[i].ColPoint, &NewDir, PrimBulls[i].GroupImIn );
						if( !Random_Range( 16 ) )
						{
							PlayPannedSfx( SFX_Ric, PrimBulls[i].ColGroup , (VECTOR *) &PrimBulls[i].ColPoint, 0.0F );
						}
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					case NME_LIGHTNING:
 						CreateNmeLightningPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
   						break;

   					case NME_LASER:
 						CreateNmeLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
   						break;

   					case NME_POWERLASER:
						CreateNmePowerLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

   					case LASER:
 						CreateLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
   						break;

   					case ORBITPULSAR:
						CreatePulsarExplosion( (VECTOR *) &PrimBulls[i].ColPoint, &NewDir, PrimBulls[i].GroupImIn );
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;

   					default:
   						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
   						break;
   				}

				if( Killed ) goto loop;
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Ship
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( HitTarget != (uint16) -1 )							/* Hit Target? */
			{

				TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
				TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
				TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
				DistToCenter = VectorLength( &TempVector );

				if( !( IsServerGame && ColPerspective != COLPERS_Forsaken ) )
					PlayPannedSfx( SFX_ShipHit, Ships[ HitTarget ].Object.Group , &Int_Point, 0.0F );

				Damage = PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];

   				switch( PrimBulls[ i ].Weapon )
   				{
   					case TROJAX:
					//	Damage =  PrimBulls[i].TrojPower / 100.0F * 512.0F; // -- unpatched? (doesn't work as it should)
					//  Damage = 32.0F + (PrimBulls[i].TrojPower / 100.0F * (256.0F-32.0F)); // -- patched
   					//	break;
   					case PULSAR:
   					case PYROLITE_RIFLE:
   					case SUSS_GUN:
   					case ORBITPULSAR:
					case NME_BULLET1:
					case NME_PULSAR:
					case NME_TROJAX:
					case NME_PYROLITE:
					case NME_SUSS_GUN:
					case FLAMES:
   						break;

					case NME_TRANSPULSE:
   					case TRANSPULSE_CANNON:
						if( !PrimBulls[i].Bounces ) // Damage /= 4.0F; // -- patched 
   						Damage /= ( ( TRANSPULSE_BOUNCES + 1 ) - PrimBulls[i].Bounces ); // -- unpatched
   						break;

					case NME_LIGHTNING:
					case NME_LASER:
   					case NME_POWERLASER:
   					case LASER:
   						TempVector.x = ( Int_Point.x - PrimBulls[ i ].Pos.x );
   						TempVector.y = ( Int_Point.y - PrimBulls[ i ].Pos.y );
   						TempVector.z = ( Int_Point.z - PrimBulls[ i ].Pos.z );
   						DistToInt = VectorLength( &TempVector );
   						if( DistToInt > LASER_MINDAMAGERANGE ) DistToInt = LASER_MINDAMAGERANGE;
   						Damage =  ( Damage * ( 1.25F - ( ( DistToInt / LASER_MINDAMAGERANGE ) / 2.0F ) ) );
   						Damage =  ( Damage * ( framelag / 3.0F ) );
   						break;
   				}

				switch( ColPerspective )
				{
					case COLPERS_Server:
						if( IsServer )
						{
   							if( PrimBulls[i].OwnerType == OWNER_SHIP )
 							{
								Server_WhoHitYou = (BYTE)PrimBulls[i].Owner;

   								if( DistToCenter >= ( SHIP_RADIUS + 1.0F ) )
   								{
   									Damage = Damage * ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
									PrimBulls[i].TrojPower *= 1.0F - ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
   								}

								if( PrimBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

   								Recoil.x = ( PrimBulls[i].Dir.x * ( Damage / 10.0F ) );
   								Recoil.y = ( PrimBulls[i].Dir.y * ( Damage / 10.0F ) );
   								Recoil.z = ( PrimBulls[i].Dir.z * ( Damage / 10.0F ) );

								if( HitTarget == WhoIAm )
								{
									HitMe( PrimBulls[i].OwnerType, PrimBulls[i].Owner, Damage, WEPTYPE_Primary, PrimBulls[i].Weapon );
									ForceExternalOneOff( WhoIAm, &Recoil );
								}
								else
								{
	   								IHitYou( (BYTE) HitTarget, Damage, &Recoil, &Int_Point, &DirVector, D2R( ( Damage / 10.0F ) ), WEPTYPE_Primary, PrimBulls[i].Weapon, ONEOFF_RECOIL );
								}
   							}


						}
						break;


					case COLPERS_Forsaken:
   						if( ( ( PrimBulls[i].OwnerType == OWNER_SHIP ) && ( PrimBulls[i].Owner == WhoIAm ) ) ||
							  ( PrimBulls[i].OwnerType != OWNER_SHIP ) )
 						{
   							if( DistToCenter >= ( SHIP_RADIUS + 1.0F ) )
   							{
   								Damage = Damage * ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
								PrimBulls[i].TrojPower *= 1.0F - ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
   							}

							if( PrimBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

   							Recoil.x = ( PrimBulls[i].Dir.x * ( Damage / 10.0F ) );
   							Recoil.y = ( PrimBulls[i].Dir.y * ( Damage / 10.0F ) );
   							Recoil.z = ( PrimBulls[i].Dir.z * ( Damage / 10.0F ) );

							if( HitTarget == WhoIAm )
							{
								HitMe( PrimBulls[i].OwnerType, PrimBulls[i].Owner, Damage, WEPTYPE_Primary, PrimBulls[i].Weapon );
								ForceExternalOneOff( WhoIAm, &Recoil );
							}
							else
							{
	   							IHitYou( (BYTE) HitTarget, Damage, &Recoil, &Int_Point, &DirVector, D2R( ( Damage / 10.0F ) ), WEPTYPE_Primary, PrimBulls[i].Weapon, ONEOFF_RECOIL );
							}
   						}
						break;

					case COLPERS_Descent:
   						if( HitTarget == WhoIAm )
 						{
   							if( DistToCenter >= ( SHIP_RADIUS + 1.0F ) )
   							{
   								Damage = Damage * ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
								PrimBulls[i].TrojPower *= 1.0F - ( ( PrimBulls[i].ColRadius - (DistToCenter - SHIP_RADIUS) ) / PrimBulls[i].ColRadius );
   							}

							if( PrimBulls[i].OwnerType == OWNER_ENEMY ) Damage *= NmeDamageModifier;

   							Recoil.x = ( PrimBulls[i].Dir.x * ( Damage / 10.0F ) );
   							Recoil.y = ( PrimBulls[i].Dir.y * ( Damage / 10.0F ) );
   							Recoil.z = ( PrimBulls[i].Dir.z * ( Damage / 10.0F ) );

							HitMe( PrimBulls[i].OwnerType, PrimBulls[i].Owner, Damage, WEPTYPE_Primary, PrimBulls[i].Weapon );
							ForceExternalOneOff( WhoIAm, &Recoil );
   						}
						break;
				}

//				if( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) )
				if( ( ( WhoIAm != HitTarget ) || ( Current_Camera_View != WhoIAm ) ) && !( IsServerGame && ColPerspective != COLPERS_Forsaken ) )
				{
					if( Ships[ HitTarget ].Object.Shield || Ships[ HitTarget ].Invul  )
					{
						switch( PrimBulls[ i ].Weapon )
						{
							case PULSAR:
							case ORBITPULSAR:
							case NME_PULSAR:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 2, 0, 255, 0 );
								break;
							case NME_TROJAX:
							case TROJAX:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 4, 128, 128, 255 );
								break;
							case NME_TRANSPULSE:
							case TRANSPULSE_CANNON:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 4, 255, 100, 100 );
								break;
							case NME_SUSS_GUN:
							case SUSS_GUN:
							case NME_PYROLITE:
							case PYROLITE_RIFLE:
							case FLAMES:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 1, 255, 100, 100 );
								break;
							case NME_LIGHTNING:
		    				case NME_LASER:
		   					case NME_POWERLASER:
		    				case LASER:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 1, 200, 200, 255 );
								break;
							case NME_BULLET1:
								CreateShieldEffect( (VECTOR *) &Ships[HitTarget].Object.Pos, &Int_Point, &Int_Point2, HitTarget, 1, 0, 255, 0 );
								break;
							default:
								break;
						}
					}
					else
					{
						switch( PrimBulls[ i ].Weapon )
						{
							case NME_BULLET1:
							case NME_PULSAR:
							case NME_TROJAX:
							case NME_TRANSPULSE:
							case NME_PYROLITE:
							case FLAMES:
							case PULSAR:
							case ORBITPULSAR:
							case TROJAX:
							case TRANSPULSE_CANNON:
							case PYROLITE_RIFLE:
								TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
								TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
								TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
								NormaliseVector( &TempVector );
								CreateColSparks( &Int_Point, &TempVector, Ships[ HitTarget ].Object.Group );
								break;

							case NME_SUSS_GUN:
							case NME_LASER:
		   					case NME_POWERLASER:
							case NME_LIGHTNING:
							case SUSS_GUN:
							case LASER:
								TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
								TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
								TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
								NormaliseVector( &TempVector );
								CreateSmallColSparks( &Int_Point, &TempVector, Ships[ HitTarget ].Object.Group );
								break;
						}
					}
				}

				TempVector.x = ( Int_Point.x - Ships[ HitTarget ].Object.Pos.x );
				TempVector.y = ( Int_Point.y - Ships[ HitTarget ].Object.Pos.y );
				TempVector.z = ( Int_Point.z - Ships[ HitTarget ].Object.Pos.z );
				NormaliseVector( &TempVector );

		  		ReflectVector( &PrimBulls[i].Dir, (NORMAL *) &TempVector, &NewDir );
	
				Killed = FALSE;

				switch( PrimBulls[ i ].Weapon )
				{
					case NME_BULLET1:
						if( Ships[ HitTarget ].Invul )
						{
							NewPos = Int_Point;
							PrimBulls[i].Dir = NewDir;
							PrimBulls[i].ColFlag = 0;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case NME_PULSAR:
						if( Ships[ HitTarget ].Invul )
						{
							ChangePulsarDir( i, &NewDir );
							NewPos = Int_Point;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CreateNmePulsarExplosion( &Int_Point, &NewDir, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case PULSAR:
						if( Ships[ HitTarget ].Invul )
						{
							ChangePulsarDir( i, &NewDir );
							NewPos = Int_Point;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CreatePulsarExplosion( &Int_Point, &NewDir, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case NME_PYROLITE:
					case PYROLITE_RIFLE:
						NewPos = Int_Point;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						PrimBulls[i].Dir = NewDir;
						PrimBulls[i].ColFlag = 0;
						break;

					case FLAMES:
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case NME_TROJAX:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							if( Ships[ HitTarget ].Invul )
							{
								CreateNmeTrojaxExplosion( &Int_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
								ChangeTrojaxDir( i, &NewDir );
								NewPos = Int_Point;
								MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
								MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
								MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
							}
							else
							{
								CreateNmeTrojaxExplosion( &Int_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
								CleanUpPrimBull( i, FALSE );
								Killed = TRUE;
							}
						}
						break;
					case TROJAX:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							if( Ships[ HitTarget ].Invul )
							{
								CreateTrojaxExplosion( &Int_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
								ChangeTrojaxDir( i, &NewDir );
								NewPos = Int_Point;
								MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
								MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
								MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
							}
							else
							{
								CreateTrojaxExplosion( &Int_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
								CleanUpPrimBull( i, FALSE );
								Killed = TRUE;
							}
						}
						break;

					case NME_TRANSPULSE:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateNmeArcExplosion( &Int_Point, &TempVector, PrimBulls[i].GroupImIn );

							if( Ships[ HitTarget ].Invul )
 							{
								NewPos = Int_Point;
								MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
								MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
								MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

								if( !ChangeTranspulseDir( i, &NewPos, &MoveOffsetVector, &NewDir ) )
								{
			   						CleanUpPrimBull( i, FALSE );
									Killed = TRUE;
								}
							}
							else
							{
		   						CleanUpPrimBull( i, FALSE );
								Killed = TRUE;
							}
						}
						break;
					case TRANSPULSE_CANNON:
						if( DistToCenter < ( SHIP_RADIUS + 1.0F ) )
						{
							CreateArcExplosion( &Int_Point, &TempVector, PrimBulls[i].GroupImIn );

							if( Ships[ HitTarget ].Invul )
 							{
								NewPos = Int_Point;
								MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
								MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
								MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

								if( !ChangeTranspulseDir( i, &NewPos, &MoveOffsetVector, &NewDir ) )
								{
			   						CleanUpPrimBull( i, FALSE );
									Killed = TRUE;
								}
							}
							else
							{
		   						CleanUpPrimBull( i, FALSE );
								Killed = TRUE;
							}
						}
						break;

					case NME_SUSS_GUN:
						if( Ships[ HitTarget ].Invul )
						{
							ChangeSussgunDir( i, &NewDir );
							NewPos = Int_Point;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CreateNmeShrapnelExplosion( &Int_Point, &NewDir, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case SUSS_GUN:
						if( Ships[ HitTarget ].Invul )
						{
							ChangeSussgunDir( i, &NewDir );
							NewPos = Int_Point;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CreateShrapnelExplosion( &Int_Point, &NewDir, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

    				case NME_LIGHTNING:
						CreateNmeLightningPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

    				case NME_LASER:
						CreateNmeLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

   					case NME_POWERLASER:
						CreateNmePowerLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

    				case LASER:
						CreateLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case ORBITPULSAR:
						if( Ships[ HitTarget ].Invul )
						{
							ChangeOrbitPulsarDir( i, &NewDir );
							NewPos = Int_Point;
							MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
							MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
							MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );
						}
						else
						{
							CreatePulsarExplosion( &Int_Point, &NewDir, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;
					default:
						break;
				}

				if( Killed )
				{
					goto loop;
				}
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Hit Enemy
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			if( HitEnemy != NULL )
			{
				TempVector.x = ( EInt_Point.x - HitEnemy->Object.Pos.x );
				TempVector.y = ( EInt_Point.y - HitEnemy->Object.Pos.y );
				TempVector.z = ( EInt_Point.z - HitEnemy->Object.Pos.z );
				DistToCenter = VectorLength( &TempVector );
				NormaliseVector( &TempVector );

		  		ReflectVector( &PrimBulls[i].Dir, (NORMAL *) &TempVector, &NewDir );

				Damage = PrimaryWeaponAttribs[ PrimBulls[i].Weapon ].Damage[ PrimBulls[i].PowerLevel ];
				if( ( PrimBulls[ i ].Weapon == LASER ) ||
					( PrimBulls[ i ].Weapon == NME_LASER ) ||
					( PrimBulls[ i ].Weapon == NME_POWERLASER ) )
				{
					Damage *= ( framelag / 3.0F );
				}

				if( DistToCenter >= ( EnemyTypes[ HitEnemy->Type ].Radius + 1.0F ) ) Damage = ( Damage / 10.0F ) * framelag;

				if( !( HitEnemy->Object.Flags & SHIP_Invul ) )
				{
					if( DamageEnemy( HitEnemy , -Damage , &EInt_Point , &PrimBulls[i].Dir , PrimBulls[ i ].Speed, PrimBulls[ i ].Owner ,PrimBulls[ i ].OwnerType ) )
					{
						CreateNewExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						KillUsedEnemy( HitEnemy );
					}
					else
					{
						CreateColSparks( &EInt_Point, &TempVector, HitEnemy->Object.Group );
					}
				}
				else
				{
					if( !Random_Range( 4 ) )
					{
						PlayPannedSfx( SFX_Ric, PrimBulls[i].ColGroup , (VECTOR *) &PrimBulls[i].ColPoint, 0.0F );
					}
				}

				Killed = FALSE;

				switch( PrimBulls[ i ].Weapon )
				{
					case NME_BULLET1:
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case NME_PULSAR:
						CreateNmePulsarExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case PULSAR:
						CreatePulsarExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case NME_PYROLITE:
					case PYROLITE_RIFLE:
						NewPos = EInt_Point;
						MoveOffsetVector.x = ( NewPos.x - PrimBulls[ i ].Pos.x );						/* Dir Vector to NewPosition */
						MoveOffsetVector.y = ( NewPos.y - PrimBulls[ i ].Pos.y );
						MoveOffsetVector.z = ( NewPos.z - PrimBulls[ i ].Pos.z );

						PrimBulls[i].Dir = NewDir;
						PrimBulls[i].ColFlag = 0;
						break;

					case FLAMES:
						CleanUpPrimBull( i, FALSE );
						break;

					case NME_TROJAX:
						if( DistToCenter < ( EnemyTypes[ HitEnemy->Type ].Radius + 1.0F ) )
						{
							CreateNmeTrojaxExplosion( &EInt_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case TROJAX:
						if( DistToCenter < ( EnemyTypes[ HitEnemy->Type ].Radius + 1.0F ) )
						{
							CreateTrojaxExplosion( &EInt_Point, &TempVector, PrimBulls[i].fmpoly, 1, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;
					case NME_TRANSPULSE:
						if( DistToCenter < ( EnemyTypes[ HitEnemy->Type ].Radius + 1.0F ) )
						{
							CreateNmeArcExplosion( &EInt_Point, &TempVector, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case TRANSPULSE_CANNON:
						if( DistToCenter < ( EnemyTypes[ HitEnemy->Type ].Radius + 1.0F ) )
						{
							CreateArcExplosion( &EInt_Point, &TempVector, PrimBulls[i].GroupImIn );
							CleanUpPrimBull( i, FALSE );
							Killed = TRUE;
						}
						break;

					case NME_SUSS_GUN:
						CreateNmeShrapnelExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					case SUSS_GUN:
						CreateShrapnelExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

    				case NME_LIGHTNING:
						CreateNmeLightningPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

    				case NME_LASER:
						CreateNmeLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

   					case NME_POWERLASER:
						CreateNmePowerLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

    				case LASER:
						CreateLaserPulse( i, DistToInt, &NewDir, PrimBulls[i].GroupImIn, PrimBulls[i].ColGroup );
						Killed = TRUE;
						break;

					case ORBITPULSAR:
						CreatePulsarExplosion( &EInt_Point, &NewDir, PrimBulls[i].GroupImIn );
						CleanUpPrimBull( i, FALSE );
						Killed = TRUE;
						break;

					default:
						break;
				}

				if( Killed )
				{
					goto loop;
				}
			}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			Done all collision checks
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, &PrimBulls[i].Pos, PrimBulls[i].GroupImIn, &MoveOffsetVector );

			PrimBulls[i].Pos = NewPos;

			if ( PrimBulls[i].light != (uint16) -1 )
			{
				XLights[PrimBulls[i].light].Group = PrimBulls[i].GroupImIn;
				XLights[PrimBulls[i].light].Pos = PrimBulls[i].Pos;
			}

			if( PrimBulls[i].line != (uint16) -1 )
			{
				Lines[ PrimBulls[i].line ].EndPos = Lines[ PrimBulls[i].line ].StartPos;
				Lines[ PrimBulls[i].line ].StartPos = PrimBulls[i].Pos;
				Lines[ PrimBulls[i].line ].Group = PrimBulls[i].GroupImIn;
			}

			if ( PrimBulls[i].fmpoly != (uint16) -1 )
			{
				fmpoly = PrimBulls[i].fmpoly;

				for( Count = 0; Count < PrimBulls[i].numfmpolys; Count++ )
				{
					nextfmpoly = FmPolys[ fmpoly ].Prev;
			
					if ( fmpoly != (uint16) -1 )
					{
						FmPolys[ fmpoly ].Pos.x += MoveOffsetVector.x; //(PrimBulls[i].Dir.x * Speed);
						FmPolys[ fmpoly ].Pos.y += MoveOffsetVector.y; //(PrimBulls[i].Dir.y * Speed);
						FmPolys[ fmpoly ].Pos.z += MoveOffsetVector.z; //(PrimBulls[i].Dir.z * Speed);
						FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					}
			
					fmpoly = nextfmpoly;
				}
			}

			if( ( PrimBulls[i].poly != (uint16) -1 ) &&
				( PrimBulls[i].Weapon != LASER ) &&
				( PrimBulls[i].Weapon != NME_LASER ) &&
				( PrimBulls[i].Weapon != NME_POWERLASER ) &&
				( PrimBulls[i].Weapon != NME_LIGHTNING ) )
			{
				poly = PrimBulls[i].poly;

				for( Count = 0; Count < PrimBulls[i].numpolys; Count++ )
				{
					nextpoly = Polys[ poly ].Prev;

					if( poly != (uint16) -1 )
					{
						Polys[ poly ].Pos1.x += MoveOffsetVector.x;
						Polys[ poly ].Pos1.y += MoveOffsetVector.y;
						Polys[ poly ].Pos1.z += MoveOffsetVector.z;
						Polys[ poly ].Pos2.x += MoveOffsetVector.x;
						Polys[ poly ].Pos2.y += MoveOffsetVector.y;
						Polys[ poly ].Pos2.z += MoveOffsetVector.z;
						Polys[ poly ].Pos3.x += MoveOffsetVector.x;
						Polys[ poly ].Pos3.y += MoveOffsetVector.y;
						Polys[ poly ].Pos3.z += MoveOffsetVector.z;
						Polys[ poly ].Pos4.x += MoveOffsetVector.x;
						Polys[ poly ].Pos4.y += MoveOffsetVector.y;
						Polys[ poly ].Pos4.z += MoveOffsetVector.z;
						Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					}
			
					poly = nextpoly;
				}
			}

		}
		else
		{
			switch( PrimBulls[ i ].Weapon )
			{
				case NME_LIGHTNING:
				case NME_LASER:
				case NME_POWERLASER:
				case LASER:
					CleanUpPrimBull( i, FALSE );
					break;

				case NME_SUSS_GUN:
					CreateNmeShrapnelExplosion( (VECTOR *) &PrimBulls[i].Pos, &PrimBulls[i].Dir, PrimBulls[i].GroupImIn );
					CleanUpPrimBull( i, FALSE );
					break;

				case SUSS_GUN:
					CreateShrapnelExplosion( (VECTOR *) &PrimBulls[i].Pos, &PrimBulls[i].Dir, PrimBulls[i].GroupImIn );
					CleanUpPrimBull( i, FALSE );
					break;

				default:
					CleanUpPrimBull( i, TRUE );
					break;
			}
		}
loop:;
		i = nextprim;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Trojax Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		FmPoly
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group )
{
	uint16	fmpoly;
	VECTOR	TempUp;
	QUAT	TempQuat;
	MATRIX	TempMat;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		if( FmPoly != (uint16) -1 ) NumSparks = (int16) ( ( FmPolys[ FmPoly ].RotSpeed / 5.0F ) + 5 );
		else NumSparks = ( Random_Range( 4 ) + 4 );
	
	   	fmpoly = FindFreeFmPoly();
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			if( Type )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].SeqNum = FM_TROJAX_EXP;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].R = 128;
				FmPolys[ fmpoly ].G = 128;
				FmPolys[ fmpoly ].B = 128;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
				if( FmPoly != (uint16) -1 ) FmPolys[ fmpoly ].RotSpeed = FmPolys[ FmPoly ].RotSpeed;
				else FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].ysize = TROJAX_EXPSIZE;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
			else
			{
				QuatFrom2Vectors( &TempQuat, &Forward, Dir );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideUp, &TempUp );
		
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].SeqNum = FM_TROJAX_EXP;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_DIRCONST;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
		   		FmPolys[ fmpoly ].DirVector = *Dir;
		   		FmPolys[ fmpoly ].UpVector = TempUp;
				FmPolys[ fmpoly ].R = 128;
				FmPolys[ fmpoly ].G = 128;
				FmPolys[ fmpoly ].B = 128;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				if( FmPoly != (uint16) -1 ) FmPolys[ fmpoly ].RotSpeed = FmPolys[ FmPoly ].RotSpeed;
				else FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].ysize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].Group = Group;
				AddFmPolyToTPage( fmpoly, GetTPage( Trojax_Exp_Header, 0 ) );
			}
	   	}
	
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 32;
				FmPolys[ fmpoly ].B = 32;
		   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Trojax Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		FmPoly
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmeTrojaxExplosion( VECTOR * Pos, VECTOR * Dir, uint16 FmPoly, int16 Type, uint16 Group )
{
	uint16	fmpoly;
	VECTOR	TempUp;
	QUAT	TempQuat;
	MATRIX	TempMat;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		if( FmPoly != (uint16) -1 ) NumSparks = (int16) ( ( FmPolys[ FmPoly ].RotSpeed / 5.0F ) + 5 );
		else NumSparks = ( Random_Range( 4 ) + 4 );
	
	   	fmpoly = FindFreeFmPoly();
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			if( Type )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].SeqNum = FM_TROJAX_EXP;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].R = 128;
				FmPolys[ fmpoly ].G = 16;
				FmPolys[ fmpoly ].B = 16;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
				if( FmPoly != (uint16) -1 ) FmPolys[ fmpoly ].RotSpeed = FmPolys[ FmPoly ].RotSpeed;
				else FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].ysize = TROJAX_EXPSIZE;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
			else
			{
				QuatFrom2Vectors( &TempQuat, &Forward, Dir );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideUp, &TempUp );
		
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		   		FmPolys[ fmpoly ].SeqNum = FM_TROJAX_EXP;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		   		FmPolys[ fmpoly ].Flags = FM_FLAG_DIRCONST;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x = 0.0F;
		   		FmPolys[ fmpoly ].Dir.y = 0.0F;
		   		FmPolys[ fmpoly ].Dir.z = 0.0F;
		   		FmPolys[ fmpoly ].DirVector = *Dir;
		   		FmPolys[ fmpoly ].UpVector = TempUp;
				FmPolys[ fmpoly ].R = 128;
				FmPolys[ fmpoly ].G = 16;
				FmPolys[ fmpoly ].B = 16;
		   		FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Rot = 0.0F;
				if( FmPoly != (uint16) -1 ) FmPolys[ fmpoly ].RotSpeed = FmPolys[ FmPoly ].RotSpeed;
				else FmPolys[ fmpoly ].RotSpeed = 0.0F;
				FmPolys[ fmpoly ].xsize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].ysize = TROJAX_EXPSIZE;
				FmPolys[ fmpoly ].Group = Group;
				AddFmPolyToTPage( fmpoly, GetTPage( Trojax_Exp_Header, 0 ) );
			}
	   	}
	
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 32;
				FmPolys[ fmpoly ].B = 32;
		   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Pulsar Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreatePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_PULSAR_EXP;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Pulsar_Exp_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 128;
			FmPolys[ fmpoly ].G = 128;
			FmPolys[ fmpoly ].B = 128;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = PULSAR_EXPSIZE;
			FmPolys[ fmpoly ].ysize = PULSAR_EXPSIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Pulsar Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmePulsarExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_PULSAR_EXP;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Pulsar_Exp_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 32;
			FmPolys[ fmpoly ].B = 32;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = PULSAR_EXPSIZE;
			FmPolys[ fmpoly ].ysize = PULSAR_EXPSIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Mushroom Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateMushroomExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	VECTOR	TempUpVector;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_MUSHROOM;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Mushroom_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = 8.0F;
			FmPolys[ fmpoly ].ysize = 8.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}

	TempUpVector.x = 0.0F;
	TempUpVector.y = 1.0F;
	TempUpVector.z = 0.0F;
	CreateSmokeRing( Pos, Dir, &TempUpVector, Group );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create New Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNewExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	VECTOR	TempUpVector;
	uint16	rapit;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_MUSHROOM;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &BigExp1_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = 5.0F;
			FmPolys[ fmpoly ].ysize = 5.0F;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}

	TempUpVector.x = 0.0F;
	TempUpVector.y = 1.0F;
	TempUpVector.z = 0.0F;
	CreateSmokeRing( Pos, Dir, &TempUpVector, Group );

	rapit = Random_Range( 5 );
	switch( rapit )
	{
	case 0:
		PlayPannedSfx( SFX_Exp2, Group , Pos, 0.0F );
		break;
	case 1:
		PlayPannedSfx( SFX_Exp3, Group , Pos, 0.0F );
		break;
	case 2:
		PlayPannedSfx( SFX_Exp4, Group , Pos, 0.0F );
		break;
	case 3:
		PlayPannedSfx( SFX_Exp5, Group , Pos, 0.0F );
		break;
	case 4:
	default:
		PlayPannedSfx( SFX_Exp6, Group , Pos, 0.0F );
		break;
	}

}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Shrapnel Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_SHRAPNEL_EXP;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 128;
			FmPolys[ fmpoly ].G = 128;
			FmPolys[ fmpoly ].B = 128;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = SUSSGUN_FLARESIZE;
			FmPolys[ fmpoly ].ysize = SUSSGUN_FLARESIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Shrapnel Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmeShrapnelExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_SHRAPNEL_EXP;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 64;
			FmPolys[ fmpoly ].G = 128;
			FmPolys[ fmpoly ].B = 64;
	   		FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = SUSSGUN_FLARESIZE;
			FmPolys[ fmpoly ].ysize = SUSSGUN_FLARESIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Clean up and kill a Prim Bull
	Input		:	uint16	Prim Bullet Number
				:	BOOL	Kill Light ( TRUE/FALSE )
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CleanUpPrimBull( uint16 i, BOOL LightFlag  )
{
	uint16	line;
	uint16	light;
	uint16	fmpoly;
	uint16	nextfmpoly;
	uint16	poly;
	uint16	nextpoly;
	int16	Count;

	line = PrimBulls[i].line;
	light = PrimBulls[i].light;
	fmpoly = PrimBulls[i].fmpoly;
	poly = PrimBulls[i].poly;

	if ( line != (uint16) -1 )
	{
		KillUsedLine( line );
		PrimBulls[i].line = (uint16) -1;
	}
	
	if ( light != (uint16) -1 )
	{
		if( LightFlag )
		{
			KillUsedXLight( light );
			PrimBulls[i].light = (uint16) -1;
		}
		else
		{
			SetLightDie(PrimBulls[i].light);
			PrimBulls[i].light = (uint16) -1;
		}
	}

	for( Count = 0; Count < PrimBulls[i].numfmpolys; Count++ )
	{
		nextfmpoly = FmPolys[ fmpoly ].Prev;
		if ( fmpoly != (uint16) -1 ) KillUsedFmPoly( fmpoly );
		fmpoly = nextfmpoly;
	}
	PrimBulls[i].numfmpolys = 0;
	PrimBulls[i].fmpoly = (uint16) -1;

	for( Count = 0; Count < PrimBulls[i].numpolys; Count++ )
	{
		nextpoly = Polys[ poly ].Prev;
		if( poly != (uint16) -1 ) KillUsedPoly( poly );
		poly = nextpoly;
	}
	PrimBulls[i].numpolys = 0;
	PrimBulls[i].poly = (uint16) -1;

	PrimBulls[i].LifeCount = 0.0F;

	KillUsedPrimBull( i );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Init 1 Primary Bullet..Mainly for Dplay...
	Input		:	uint16		OwnerType
				:	uint16		OwnerID
				:	uint16		BulletID
				:	uint16		Weapon
				:	uint16		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	Offset
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	int16		PowerLevel
				:	float		PowerLevel ( Charging )
				:	BOOL		NetFlag ( Send to everyone )
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 InitOnePrimBull( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag )
{
	uint16		i;
	uint16		light;
	uint16		fmpoly;
	uint16		poly;
	float		Trail_Length;
	float		Size, Half;
	VECTOR		LeftVector;
	QUAT		TempQuat;

#ifdef SHADOWTEST
	CreateSpotLight( (uint16) WhoIAm, SHIP_RADIUS, &Mloadheader );
#endif

	if( MyGameStatus != STATUS_SinglePlayer )
	{
		if( !IsGroupVisible[ Group ] && !VisibleOverlap( Ships[ Current_Camera_View ].Object.Group, Group, NULL ) && !outside_map ) return( (uint16) -1 );
	}

	CrossProduct( Dir, Up, &LeftVector );

	i = FindFreePrimBull();

//	DebugPrintf( "ID = %d, Pos = %f,%f,%f, Offset = %f,%f%,%f, Group = %d\n", BulletID, Pos->x, Pos->y, Pos->z, Offset->x, Offset->y, Offset->z, Group );

	if( i != (uint16) -1 )
	{
		PrimBulls[i].Offset = *Offset;
		PrimBulls[i].Pos.x = ( Pos->x + Offset->x );
		PrimBulls[i].Pos.y = ( Pos->y + Offset->y );
		PrimBulls[i].Pos.z = ( Pos->z + Offset->z );
		PrimBulls[i].Dir = *Dir;
		PrimBulls[i].UpVector = *Up;
		PrimBulls[i].OwnerType = OwnerType;
		PrimBulls[i].Owner = OwnerID;
		PrimBulls[i].Weapon = Weapon;
		PrimBulls[i].ID = BulletID;
		PrimBulls[i].PowerLevel = PowerLevel;
		PrimBulls[i].ColRadius = PrimaryWeaponAttribs[ Weapon ].ColRadius[ PowerLevel ];
		PrimBulls[i].ColType = PrimaryWeaponAttribs[ Weapon ].ColType;
		PrimBulls[i].Speed =	 PrimaryWeaponAttribs[ Weapon ].Speed[ PowerLevel ];
		PrimBulls[i].LifeCount = PrimaryWeaponAttribs[ Weapon ].LifeCount;
		PrimBulls[i].ColFlag = 0;
		PrimBulls[i].ColPoint.x = MaxColDistance;
		PrimBulls[i].ColPoint.y = MaxColDistance;
		PrimBulls[i].ColPoint.z = MaxColDistance;
		PrimBulls[i].GroupImIn = Group;
		PrimBulls[i].numfmpolys = 0;
		PrimBulls[i].numpolys = 0;
		PrimBulls[i].fmpoly = (uint16) -1;
		PrimBulls[i].poly = (uint16) -1;
		PrimBulls[i].line = (uint16) -1;
		PrimBulls[i].light = (uint16) -1;
		PrimBulls[i].Bounces = 0;
		PrimBulls[i].EnemyGun = NULL;
		PrimBulls[i].FramelagAddition = 0.0F;
		PrimBulls[i].TrojPower = PLevel;

		if( ( Weapon != LASER ) &&
			( Weapon != NME_LASER ) &&
			( Weapon != NME_POWERLASER ) &&
			( Weapon != NME_LIGHTNING ) )
		{
			PrimBulls[i].GroupImIn = MoveGroup( &Mloadheader, Pos, Group, Offset );
		}

		GetLaserLocalVector( i, &PrimBulls[i].LocalDir );

		switch( Weapon )
		{
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			PULSAR
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		   	case PULSAR:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) && !BombTag )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
					}
				}

				if( PowerLevel >= PULSAR2SHOTPOWERLEVEL )
				{
					if( ( OwnerType == OWNER_SHIP ) && ( OwnerID != (uint16) -1 ) )
					{
						if( Ships[ OwnerID ].Object.Flags & SHIP_PrimToggle )
						{
							PlayPannedSfx( SFX_Pulsar, PrimBulls[i].GroupImIn, Pos, 0.0F );
						}
					}
					else
					{
						PlayPannedSfx( SFX_Pulsar, PrimBulls[i].GroupImIn, Pos, 0.0F );
					}
				}
				else 
				{
					PlayPannedSfx( SFX_Pulsar, PrimBulls[i].GroupImIn, &PrimBulls[i].Pos, 0.0F );
				}

				light = FindFreeXLight();

				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				PrimBulls[i].fmpoly = (uint16) -1;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (uint16 ) -1 )
   				{
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
   					FmPolys[ fmpoly ].Frame = 0.0F;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = PULSAR_FMSIZE;
   					FmPolys[ fmpoly ].ysize = PULSAR_FMSIZE;
   					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
   					PrimBulls[i].numfmpolys++;
   					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   				}

				Trail_Length = ( (float) -( ( PrimBulls[ i ].PowerLevel * PULSAR_TAILPOWERMUL ) + PULSAR_TAILADDMIN ) );

				poly = FindFreePoly();

				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( LeftVector.x * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( LeftVector.y * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( LeftVector.z * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( LeftVector.x * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( LeftVector.y * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( LeftVector.z * PULSAR_TAILSTART );
					Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) + ( LeftVector.x * PULSAR_TAILEND );
					Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) + ( LeftVector.y * PULSAR_TAILEND );
					Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) + ( LeftVector.z * PULSAR_TAILEND );
					Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) - ( LeftVector.x * PULSAR_TAILEND );
					Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) - ( LeftVector.y * PULSAR_TAILEND );
					Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) - ( LeftVector.z * PULSAR_TAILEND );
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 255;
					Polys[ poly ].Col1.B = 255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 255;
					Polys[ poly ].Col2.B = 255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 255;
					Polys[ poly ].Col3.B = 255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 255;
					Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ poly ].Frm_Info = &Pulsar_Trail_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_PULSAR_TRAIL;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );

				}

				poly = FindFreePoly();

				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( -Up->x * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( -Up->y * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( -Up->z * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( -Up->x * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( -Up->y * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( -Up->z * PULSAR_TAILSTART );
					Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) + ( -Up->x * PULSAR_TAILEND );
					Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) + ( -Up->y * PULSAR_TAILEND );
					Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) + ( -Up->z * PULSAR_TAILEND );
					Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) - ( -Up->x * PULSAR_TAILEND );
					Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) - ( -Up->y * PULSAR_TAILEND );
					Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) - ( -Up->z * PULSAR_TAILEND );
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 255;
					Polys[ poly ].Col1.B = 255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 255;
					Polys[ poly ].Col2.B = 255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 255;
					Polys[ poly ].Col3.B = 255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 255;
					Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ poly ].Frm_Info = &Pulsar_Trail_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_PULSAR_TRAIL;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			TROJAX
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case TROJAX:
				PrimBulls[i].PowerLevel = (int16) ( ( PLevel + ( 100.0F / ( MAXPOWERLEVELS - 1 ) ) - 1 ) / ( 100.0F / ( MAXPOWERLEVELS - 1 ) ) );
				PrimBulls[i].ColRadius = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE ) * 30.0F;

				PlayPannedSfx( SFX_Trojax, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PrimBulls[i].PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PrimBulls[i].PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PrimBulls[i].PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].Frame = 0.0F;
				  	FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
					FmPolys[ fmpoly ].DirVector = *Dir;
					QuatFrom2Vectors( &TempQuat, &Forward, Dir );
					QuatToMatrix( &TempQuat, &FmPolys[ fmpoly ].Mat );
					FmPolys[ fmpoly ].xsize = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE );	//TROJAX_SIZE;
					FmPolys[ fmpoly ].ysize = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE );	//TROJAX_SIZE;
				   	FmPolys[ fmpoly ].R = (uint8) ( 205 + ( PLevel * 0.5F ) );
				   	FmPolys[ fmpoly ].G = (uint8) ( 205 + ( PLevel * 0.5F ) );
				   	FmPolys[ fmpoly ].B = (uint8) ( 205 + ( PLevel * 0.5F ) );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = (uint8) ( 205 + ( PLevel * 0.5F ) );
#endif

					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
	
					if( PLevel == 100.0F )
					{
						FmPolys[ PrimBulls[i].fmpoly ].RotSpeed = PLevel + 10.0F;
						FmPolys[ PrimBulls[i].fmpoly ].Frm_Info = &Trojax2_Header;
						FmPolys[ PrimBulls[i].fmpoly ].SeqNum = FM_TROJAX;
						if( PrimBulls[i].light != (uint16) -1 ) XLights[ PrimBulls[i].light ].Size = ( 2048.0F * GLOBAL_SCALE );
					}
					else
					{
						FmPolys[ PrimBulls[i].fmpoly ].RotSpeed = PLevel + 10.0F;
						FmPolys[ PrimBulls[i].fmpoly ].Frm_Info = &Trojax1_Header;
						FmPolys[ PrimBulls[i].fmpoly ].SeqNum = FM_TROJAX;
						if( PrimBulls[i].light != (uint16) -1 ) XLights[ PrimBulls[i].light ].Size = ( 1536.0F * GLOBAL_SCALE );
					}
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			PYROLITE RIFLE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case PYROLITE_RIFLE:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						PyroliteAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						PyroliteAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( PyroliteAmmo < 0.0F ) PyroliteAmmo = 0.0F;
					}
				}

				if( PyroCount > 60.0F )
				{
					PyroCount = FMOD( PyroCount, 60.0F );
					PlayPannedSfx( SFX_Pyroloop, PrimBulls[i].GroupImIn, Pos, 0.0F );
				}

				if( PyroLightFlag )
				{
					PyroLightFlag = FALSE;

					light = FindFreeXLight();
					if( light != (uint16 ) -1 )					// Light attached
					{
						if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
						XLights[ light ].Pos = PrimBulls[i].Pos;
						XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
						XLights[ light ].SizeCount = 0.0F;
						XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
						XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
						XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
						XLights[ light ].Group = PrimBulls[i].GroupImIn;
						PrimBulls[i].light = light;
					}
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * PYROLITE_POWERMUL ) + PYROLITE_ADDMIN );
					FmPolys[ fmpoly ].ysize = ( ( PowerLevel * PYROLITE_POWERMUL ) + PYROLITE_ADDMIN );
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
				   	FmPolys[ fmpoly ].LifeCount = 64.0F;
				   	FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				   	FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				   	FmPolys[ fmpoly ].RotSpeed = (float) ( Random_Range( 30 ) + 20 );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 128;
				   	FmPolys[ fmpoly ].G = 128;
				   	FmPolys[ fmpoly ].B = 128;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
		   		break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			TRANSPULSE CANNON
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		   	case TRANSPULSE_CANNON:
				PrimBulls[i].Bounces = TRANSPULSE_BOUNCES;
				PrimBulls[i].ColRadius = ( ( PrimBulls[i].PowerLevel * ARC_POWERMUL ) + ARC_ADDMIN );

				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						if( MyGameStatus == STATUS_SinglePlayer )
						{
							GeneralAmmoUsed += ( 4.0F * PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ] );
							GeneralAmmo -= ( 4.0F * PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ] );
							if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
						}
						else
						{
							GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
							GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
							if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
						}
					}
				}

				PlayPannedSfx(SFX_Transpulse, PrimBulls[i].GroupImIn, Pos, 0.0F );
				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].Dir = *Dir;
					FmPolys[ fmpoly ].Speed = PrimaryWeaponAttribs[ Weapon ].Speed[ PowerLevel ];
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * TRANSPULSE_POWERMUL ) + TRANSPULSE_ADDMIN );
					FmPolys[ fmpoly ].ysize = ( ( PowerLevel * TRANSPULSE_POWERMUL ) + TRANSPULSE_ADDMIN );
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * ARC_POWERMUL ) + ARC_ADDMIN );
					FmPolys[ fmpoly ].ysize = ARC_LENGTH;
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].DirVector = *Up;
					FmPolys[ fmpoly ].UpVector = *Dir;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			SUSS_GUN
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case SUSS_GUN:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						SussGunAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						SussGunAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( SussGunAmmo < 0.0F ) SussGunAmmo = 0.0F;
					}
				}

				PrimBulls[ i ].LifeCount = ( 30.0F + Random_Range( (uint16) ( ( PowerLevel * SUSSGUNLIFE_POWERMUL ) + SUSSGUNLIFE_ADDMIN ) ) );

				PlayPannedSfx( SFX_Suss_Gun, PrimBulls[i].GroupImIn, Pos, 0.0F );

				Size = SussgunTable[ PrimBulls[i].PowerLevel ];

				Half = (float) ( ( 1.0F / ( Size / 5120.0F ) ) / 2.0F );
		   		PrimBulls[i].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );
		   		PrimBulls[i].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );
		   		PrimBulls[i].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].xsize = ( PowerLevel * SUSSGUN_POWERMUL ) + SUSSGUN_ADDMIN;
					FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			LASER
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case LASER:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;

						LaserTemperature += ( ( PowerLevel * LASER_TEMP_POWERMUL ) + LASER_TEMP_ADDMIN );

						if( LaserTemperature >= 100.0F )
						{
							LaserTemperature = 100.0F;
							LaserOverheated = 1;
							AddMessageToQue( LASER_OVERHEATED );
						}
					}
				}

				LaserDiameter = ( ( PowerLevel * LASER_WIDTH_POWERMUL ) + LASER_WIDTH_ADDMIN );

				PlayPannedSfx( SFX_Laser, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
					FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				poly = FindFreePoly();
				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1 = PrimBulls[i].Pos;
					Polys[ poly ].Pos2 = PrimBulls[i].Pos;
					Polys[ poly ].Pos3 = PrimBulls[i].Pos;
					Polys[ poly ].Pos4 = PrimBulls[i].Pos;
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 255;
					Polys[ poly ].Col1.B = 255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 255;
					Polys[ poly ].Col2.B = 255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 255;
					Polys[ poly ].Col3.B = 255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 255;
					Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
					Polys[ poly ].Frm_Info = &Laser_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_LASER;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
				}

				if( !( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) ) )	/* only other people see cross */
				{
					poly = FindFreePoly();
					if( poly != (uint16) -1 )
					{
						Polys[ poly ].Pos1 = PrimBulls[i].Pos;
						Polys[ poly ].Pos2 = PrimBulls[i].Pos;
						Polys[ poly ].Pos3 = PrimBulls[i].Pos;
						Polys[ poly ].Pos4 = PrimBulls[i].Pos;
						Polys[ poly ].Col1.R = 255;
						Polys[ poly ].Col1.G = 255;
						Polys[ poly ].Col1.B = 255;
						Polys[ poly ].Col2.R = 255;
						Polys[ poly ].Col2.G = 255;
						Polys[ poly ].Col2.B = 255;
						Polys[ poly ].Col3.R = 255;
						Polys[ poly ].Col3.G = 255;
						Polys[ poly ].Col3.B = 255;
						Polys[ poly ].Col4.R = 255;
						Polys[ poly ].Col4.G = 255;
						Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
						Polys[ poly ].Trans1 = 128;
						Polys[ poly ].Trans2 = 128;
						Polys[ poly ].Trans3 = 128;
						Polys[ poly ].Trans4 = 128;
#else
						Polys[ poly ].Trans1 = 255;
						Polys[ poly ].Trans2 = 255;
						Polys[ poly ].Trans3 = 255;
						Polys[ poly ].Trans4 = 255;
#endif
						Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
						Polys[ poly ].Frm_Info = &Laser_Header;
						Polys[ poly ].Frame = 0.0F;
						Polys[ poly ].SeqNum = POLY_LASER;
						Polys[ poly ].Group = PrimBulls[i].GroupImIn;
						PrimBulls[i].numpolys++;
						PrimBulls[i].poly = poly;

						AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			ORBIT PULSAR
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		   	case ORBITPULSAR:
				//PlayPannedSfxWithVolModify(SFX_Orbit, PrimBulls[i].GroupImIn, Pos, 0.0F, 0.75F );
				PlayPannedSfx(SFX_Orbit, PrimBulls[i].GroupImIn, Pos, 0.0F );
				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				PrimBulls[i].fmpoly = (uint16) -1;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (uint16 ) -1 )
   				{
					if( ( OwnerType == OWNER_SHIP ) )
					{
						if( ( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
						{
							FmPolys[fmpoly].R = SUPERNASHRAM_RED;
							FmPolys[fmpoly].G = SUPERNASHRAM_GREEN;
							FmPolys[fmpoly].B = SUPERNASHRAM_BLUE;
						}
					}

#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
   					FmPolys[ fmpoly ].Frame = 0.0F;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = ORBITPULSAR_FMSIZE;
   					FmPolys[ fmpoly ].ysize = ORBITPULSAR_FMSIZE;
   					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
   					PrimBulls[i].numfmpolys++;
   					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME BULLET1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_BULLET1:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) && !BombTag )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
					}
				}

				CreateMuzzleFlash( &PrimBulls[i].Pos, PrimBulls[i].GroupImIn, 5.0F );

				PlayPannedSfx( SFX_EnemyPulsar, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				PrimBulls[i].fmpoly = (uint16) -1;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (uint16 ) -1 )
   				{
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
   					FmPolys[ fmpoly ].Frame = 0.0F;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = NMEBUL1_FMSIZE;
   					FmPolys[ fmpoly ].ysize = NMEBUL1_FMSIZE;
   					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
   					PrimBulls[i].numfmpolys++;
   					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME PULSAR
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		   	case NME_PULSAR:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) && !BombTag )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
					}
				}

				PlayPannedSfx( SFX_EnemyPulsar, PrimBulls[i].GroupImIn, &PrimBulls[i].Pos, 0.0F );

				light = FindFreeXLight();

				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				PrimBulls[i].fmpoly = (uint16) -1;

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
   				if( fmpoly != (uint16 ) -1 )
   				{
   					FmPolys[ fmpoly ].LifeCount = 1000.0F;
   					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
   					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
   					FmPolys[ fmpoly ].Frame = 0.0F;
   					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
   					FmPolys[ fmpoly ].xsize = PULSAR_FMSIZE;
   					FmPolys[ fmpoly ].ysize = PULSAR_FMSIZE;
   					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 64; //255;
				   	FmPolys[ fmpoly ].B = 64; //255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
   					PrimBulls[i].numfmpolys++;
   					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   				}

				Trail_Length = ( (float) -( ( PrimBulls[ i ].PowerLevel * PULSAR_TAILPOWERMUL ) + PULSAR_TAILADDMIN ) );

				poly = FindFreePoly();

				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( LeftVector.x * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( LeftVector.y * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( LeftVector.z * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( LeftVector.x * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( LeftVector.y * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( LeftVector.z * PULSAR_TAILSTART );
					Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) + ( LeftVector.x * PULSAR_TAILEND );
					Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) + ( LeftVector.y * PULSAR_TAILEND );
					Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) + ( LeftVector.z * PULSAR_TAILEND );
					Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) - ( LeftVector.x * PULSAR_TAILEND );
					Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) - ( LeftVector.y * PULSAR_TAILEND );
					Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) - ( LeftVector.z * PULSAR_TAILEND );
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 64; //255;
					Polys[ poly ].Col1.B = 64; //255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 64; //255;
					Polys[ poly ].Col2.B = 64; //255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 64; //255;
					Polys[ poly ].Col3.B = 64; //255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 64; //255;
					Polys[ poly ].Col4.B = 64; //255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ poly ].Frm_Info = &Pulsar_Trail_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_PULSAR_TRAIL;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );

				}

				poly = FindFreePoly();

				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( -Up->x * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( -Up->y * PULSAR_TAILSTART );
					Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( -Up->z * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( -Up->x * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( -Up->y * PULSAR_TAILSTART );
					Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( -Up->z * PULSAR_TAILSTART );
					Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) + ( -Up->x * PULSAR_TAILEND );
					Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) + ( -Up->y * PULSAR_TAILEND );
					Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) + ( -Up->z * PULSAR_TAILEND );
					Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( Dir->x * Trail_Length ) - ( -Up->x * PULSAR_TAILEND );
					Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( Dir->y * Trail_Length ) - ( -Up->y * PULSAR_TAILEND );
					Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( Dir->z * Trail_Length ) - ( -Up->z * PULSAR_TAILEND );
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 64; //255;
					Polys[ poly ].Col1.B = 64; //255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 64; //255;
					Polys[ poly ].Col2.B = 64; //255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 64; //255;
					Polys[ poly ].Col3.B = 64; //255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 64; //255;
					Polys[ poly ].Col4.B = 64; //255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ poly ].Frm_Info = &Pulsar_Trail_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_PULSAR_TRAIL;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME TROJAX
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_TROJAX:
				PrimBulls[i].PowerLevel = (int16) ( ( PLevel + ( 100.0F / ( MAXPOWERLEVELS - 1 ) ) - 1 ) / ( 100.0F / ( MAXPOWERLEVELS - 1 ) ) );
				PrimBulls[i].ColRadius = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE ) * 30.0F;

				PlayPannedSfx( SFX_Trojax, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PrimBulls[i].PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PrimBulls[i].PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PrimBulls[i].PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].Frame = 0.0F;
				  	FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
					FmPolys[ fmpoly ].DirVector = *Dir;
					QuatFrom2Vectors( &TempQuat, &Forward, Dir );
					QuatToMatrix( &TempQuat, &FmPolys[ fmpoly ].Mat );
					FmPolys[ fmpoly ].xsize = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE );	//TROJAX_SIZE;
					FmPolys[ fmpoly ].ysize = ( ( 3.0F + ( PLevel * 0.075F ) ) * GLOBAL_SCALE );	//TROJAX_SIZE;
				   	FmPolys[ fmpoly ].R = (uint8) ( 205 + ( PLevel * 0.5F ) );
				   	FmPolys[ fmpoly ].G = 32; //(uint8) ( 205 + ( PLevel * 0.5F ) );
				   	FmPolys[ fmpoly ].B = 32; //(uint8) ( 205 + ( PLevel * 0.5F ) );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = (uint8) ( 205 + ( PLevel * 0.5F ) );
#endif

					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
	
					if( PLevel == 100.0F )
					{
						FmPolys[ PrimBulls[i].fmpoly ].RotSpeed = PLevel + 10.0F;
						FmPolys[ PrimBulls[i].fmpoly ].Frm_Info = &Trojax2_Header;
						FmPolys[ PrimBulls[i].fmpoly ].SeqNum = FM_TROJAX;
						if( PrimBulls[i].light != (uint16) -1 ) XLights[ PrimBulls[i].light ].Size = ( 2048.0F * GLOBAL_SCALE );
					}
					else
					{
						FmPolys[ PrimBulls[i].fmpoly ].RotSpeed = PLevel + 10.0F;
						FmPolys[ PrimBulls[i].fmpoly ].Frm_Info = &Trojax1_Header;
						FmPolys[ PrimBulls[i].fmpoly ].SeqNum = FM_TROJAX;
						if( PrimBulls[i].light != (uint16) -1 ) XLights[ PrimBulls[i].light ].Size = ( 1536.0F * GLOBAL_SCALE );
					}
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME PYROLITE RIFLE
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_PYROLITE:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						PyroliteAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						PyroliteAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( PyroliteAmmo < 0.0F ) PyroliteAmmo = 0.0F;
					}
				}

				if( PyroLightFlag )
				{
					PyroLightFlag = FALSE;

					light = FindFreeXLight();
					if( light != (uint16 ) -1 )					// Light attached
					{
						if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
						XLights[ light ].Pos = PrimBulls[i].Pos;
						XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
						XLights[ light ].SizeCount = 0.0F;
						XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
						XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
						XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
						XLights[ light ].Group = PrimBulls[i].GroupImIn;
						PrimBulls[i].light = light;
					}
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * PYROLITE_POWERMUL ) + PYROLITE_ADDMIN );
					FmPolys[ fmpoly ].ysize = ( ( PowerLevel * PYROLITE_POWERMUL ) + PYROLITE_ADDMIN );
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
				   	FmPolys[ fmpoly ].LifeCount = 64.0F;
				   	FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				   	FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				   	FmPolys[ fmpoly ].RotSpeed = (float) ( Random_Range( 30 ) + 20 );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 32; //128;
				   	FmPolys[ fmpoly ].G = 32; //128;
				   	FmPolys[ fmpoly ].B = 255; //128
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
		   		break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME TRANSPULSE CANNON
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
		   	case NME_TRANSPULSE:
				PrimBulls[i].Bounces = TRANSPULSE_BOUNCES;
				PrimBulls[i].ColRadius = ( ( PrimBulls[i].PowerLevel * ARC_POWERMUL ) + ARC_ADDMIN );

				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;
					}
				}

				PlayPannedSfx(SFX_Transpulse, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].Dir = *Dir;
					FmPolys[ fmpoly ].Speed = PrimaryWeaponAttribs[ Weapon ].Speed[ PowerLevel ];
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * TRANSPULSE_POWERMUL ) + TRANSPULSE_ADDMIN );
					FmPolys[ fmpoly ].ysize = ( ( PowerLevel * TRANSPULSE_POWERMUL ) + TRANSPULSE_ADDMIN );
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 64; //255;
				   	FmPolys[ fmpoly ].G = 255; //255;
				   	FmPolys[ fmpoly ].B = 64;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = ( FM_FLAG_DIRCONST | FM_FLAG_TWOSIDED );
					FmPolys[ fmpoly ].xsize = ( ( PowerLevel * ARC_POWERMUL ) + ARC_ADDMIN );
					FmPolys[ fmpoly ].ysize = ARC_LENGTH;
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].DirVector = *Up;
					FmPolys[ fmpoly ].UpVector = *Dir;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 64; //255;
				   	FmPolys[ fmpoly ].G = 255; //255;
				   	FmPolys[ fmpoly ].B = 64;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME SUSS_GUN
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_SUSS_GUN:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						SussGunAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						SussGunAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( SussGunAmmo < 0.0F ) SussGunAmmo = 0.0F;
					}
				}

				PrimBulls[ i ].LifeCount = ( 30.0F + Random_Range( (uint16) ( ( PowerLevel * SUSSGUNLIFE_POWERMUL ) + SUSSGUNLIFE_ADDMIN ) ) );

				PlayPannedSfx( SFX_EnemySussGun, PrimBulls[i].GroupImIn, Pos, 0.0F );

				Size = SussgunTable[ PrimBulls[i].PowerLevel ];

				Half = (float) ( ( 1.0F / ( Size / 5120.0F ) ) / 2.0F );
		   		PrimBulls[i].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );
		   		PrimBulls[i].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );
		   		PrimBulls[i].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / Size ) - Half );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].xsize = ( PowerLevel * SUSSGUN_POWERMUL ) + SUSSGUN_ADDMIN;
					FmPolys[ fmpoly ].ysize = FmPolys[ fmpoly ].xsize;
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
				   	FmPolys[ fmpoly ].R = 0; //255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 0; //255;
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME_LASER
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_LASER:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;

						LaserTemperature += ( ( PowerLevel * LASER_TEMP_POWERMUL ) + LASER_TEMP_ADDMIN );

						if( LaserTemperature >= 100.0F )
						{
							LaserTemperature = 100.0F;
							LaserOverheated = 1;
							AddMessageToQue( LASER_OVERHEATED );
						}
					}
				}

				LaserDiameter = ( ( PowerLevel * LASER_WIDTH_POWERMUL ) + LASER_WIDTH_ADDMIN );

				PlayPannedSfx( SFX_Laser, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
					FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 16; // 255;
				   	FmPolys[ fmpoly ].G = 255; //255;
				   	FmPolys[ fmpoly ].B = 16; //255;
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				poly = FindFreePoly();
				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1 = PrimBulls[i].Pos;
					Polys[ poly ].Pos2 = PrimBulls[i].Pos;
					Polys[ poly ].Pos3 = PrimBulls[i].Pos;
					Polys[ poly ].Pos4 = PrimBulls[i].Pos;
					Polys[ poly ].Col1.R = 16; //255
					Polys[ poly ].Col1.G = 255; //255;
					Polys[ poly ].Col1.B = 16; //255;
					Polys[ poly ].Col2.R = 16;//255;
					Polys[ poly ].Col2.G = 255; //255;
					Polys[ poly ].Col2.B = 16; //255;
					Polys[ poly ].Col3.R = 16; //255;
					Polys[ poly ].Col3.G = 255; //255;
					Polys[ poly ].Col3.B = 16; //255;
					Polys[ poly ].Col4.R = 16; //255;
					Polys[ poly ].Col4.G = 255; //255;
					Polys[ poly ].Col4.B = 16; //255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
					Polys[ poly ].Frm_Info = &Laser_Header;
					Polys[ poly ].Frame = 0.0F;
					Polys[ poly ].SeqNum = POLY_LASER;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
				}

				if( !( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) ) )	/* only other people see cross */
				{
					poly = FindFreePoly();
					if( poly != (uint16) -1 )
					{
						Polys[ poly ].Pos1 = PrimBulls[i].Pos;
						Polys[ poly ].Pos2 = PrimBulls[i].Pos;
						Polys[ poly ].Pos3 = PrimBulls[i].Pos;
						Polys[ poly ].Pos4 = PrimBulls[i].Pos;
						Polys[ poly ].Col1.R = 16; //255
						Polys[ poly ].Col1.G = 255; //255;
						Polys[ poly ].Col1.B = 16; //255; 
						Polys[ poly ].Col2.R = 16;//255;
						Polys[ poly ].Col2.G = 255; //255;
						Polys[ poly ].Col2.B = 16; //255; 
						Polys[ poly ].Col3.R = 16; //255;
						Polys[ poly ].Col3.G = 255; //255;
						Polys[ poly ].Col3.B = 16; //255; 
						Polys[ poly ].Col4.R = 16; //255;
						Polys[ poly ].Col4.G = 255; //255;
						Polys[ poly ].Col4.B = 16; //255; 
#if ACTUAL_TRANS
						Polys[ poly ].Trans1 = 128;
						Polys[ poly ].Trans2 = 128;
						Polys[ poly ].Trans3 = 128;
						Polys[ poly ].Trans4 = 128;
#else
						Polys[ poly ].Trans1 = 255;
						Polys[ poly ].Trans2 = 255;
						Polys[ poly ].Trans3 = 255;
						Polys[ poly ].Trans4 = 255;
#endif
						Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
						Polys[ poly ].Frm_Info = &Laser_Header;
						Polys[ poly ].Frame = 0.0F;
						Polys[ poly ].SeqNum = POLY_LASER;
						Polys[ poly ].Group = PrimBulls[i].GroupImIn;
						PrimBulls[i].numpolys++;
						PrimBulls[i].poly = poly;

						AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME_POWERLASER
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_POWERLASER:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;

						LaserTemperature += ( ( PowerLevel * LASER_TEMP_POWERMUL ) + LASER_TEMP_ADDMIN );

						if( LaserTemperature >= 100.0F )
						{
							LaserTemperature = 100.0F;
							LaserOverheated = 1;
							AddMessageToQue( LASER_OVERHEATED );
						}
					}
				}

				LaserDiameter = ( ( PowerLevel * PLASER_WIDTH_POWERMUL ) + PLASER_WIDTH_ADDMIN );

				PlayPannedSfx( SFX_PhotonTorpedo, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
					FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}

				poly = FindFreePoly();
				if( poly != (uint16) -1 )
				{
					Polys[ poly ].Pos1 = PrimBulls[i].Pos;
					Polys[ poly ].Pos2 = PrimBulls[i].Pos;
					Polys[ poly ].Pos3 = PrimBulls[i].Pos;
					Polys[ poly ].Pos4 = PrimBulls[i].Pos;
					Polys[ poly ].Col1.R = 255;
					Polys[ poly ].Col1.G = 255;
					Polys[ poly ].Col1.B = 255;
					Polys[ poly ].Col2.R = 255;
					Polys[ poly ].Col2.G = 255;
					Polys[ poly ].Col2.B = 255;
					Polys[ poly ].Col3.R = 255;
					Polys[ poly ].Col3.G = 255;
					Polys[ poly ].Col3.B = 255;
					Polys[ poly ].Col4.R = 255;
					Polys[ poly ].Col4.G = 255;
					Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
					Polys[ poly ].Trans1 = 128;
					Polys[ poly ].Trans2 = 128;
					Polys[ poly ].Trans3 = 128;
					Polys[ poly ].Trans4 = 128;
#else
					Polys[ poly ].Trans1 = 255;
					Polys[ poly ].Trans2 = 255;
					Polys[ poly ].Trans3 = 255;
					Polys[ poly ].Trans4 = 255;
#endif
					Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
					Polys[ poly ].Frm_Info = &Laser_Header;
					Polys[ poly ].Frame = 0.0F;
			   		Polys[ poly ].AnimSpeed = 48.0F;
					Polys[ poly ].SeqNum = POLY_POWERLASER;
					Polys[ poly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numpolys++;
					PrimBulls[i].poly = poly;

					AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
				}

				if( !( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) ) )	/* only other people see cross */
				{
					poly = FindFreePoly();
					if( poly != (uint16) -1 )
					{
						Polys[ poly ].Pos1 = PrimBulls[i].Pos;
						Polys[ poly ].Pos2 = PrimBulls[i].Pos;
						Polys[ poly ].Pos3 = PrimBulls[i].Pos;
						Polys[ poly ].Pos4 = PrimBulls[i].Pos;
						Polys[ poly ].Col1.R = 255;
						Polys[ poly ].Col1.G = 255;
						Polys[ poly ].Col1.B = 255; 
						Polys[ poly ].Col2.R = 255;
						Polys[ poly ].Col2.G = 255;
						Polys[ poly ].Col2.B = 255; 
						Polys[ poly ].Col3.R = 255;
						Polys[ poly ].Col3.G = 255;
						Polys[ poly ].Col3.B = 255; 
						Polys[ poly ].Col4.R = 255;
						Polys[ poly ].Col4.G = 255;
						Polys[ poly ].Col4.B = 255; 
#if ACTUAL_TRANS
						Polys[ poly ].Trans1 = 128;
						Polys[ poly ].Trans2 = 128;
						Polys[ poly ].Trans3 = 128;
						Polys[ poly ].Trans4 = 128;
#else
						Polys[ poly ].Trans1 = 255;
						Polys[ poly ].Trans2 = 255;
						Polys[ poly ].Trans3 = 255;
						Polys[ poly ].Trans4 = 255;
#endif
						Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
						Polys[ poly ].Frm_Info = &Laser_Header;
						Polys[ poly ].Frame = 0.0F;
				   		Polys[ poly ].AnimSpeed = 48.0F;
						Polys[ poly ].SeqNum = POLY_POWERLASER;
						Polys[ poly ].Group = PrimBulls[i].GroupImIn;
						PrimBulls[i].numpolys++;
						PrimBulls[i].poly = poly;

						AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
					}
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			NME_LIGHTNING
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case NME_LIGHTNING:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						GeneralAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						GeneralAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( GeneralAmmo < 0.0F ) GeneralAmmo = 0.0F;

						LaserTemperature += ( ( PowerLevel * LASER_TEMP_POWERMUL ) + LASER_TEMP_ADDMIN );

						if( LaserTemperature >= 100.0F )
						{
							LaserTemperature = 100.0F;
							LaserOverheated = 1;
							AddMessageToQue( LASER_OVERHEATED );
						}
					}
				}

				LaserDiameter = ( ( PowerLevel * LASER_WIDTH_POWERMUL ) + LASER_WIDTH_ADDMIN );

				PlayPannedSfx( SFX_Laser, PrimBulls[i].GroupImIn, Pos, 0.0F );

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = FM_ARC2;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
					FmPolys[ fmpoly ].xsize = ( 6.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 6.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
					FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
				break;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
			FLAMES
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
			case FLAMES:
				if( ( OwnerType == OWNER_SHIP ) && ( OwnerID == WhoIAm ) )
				{
					if( !GodMode && !( Ships[ OwnerID ].Object.Flags & SHIP_SuperNashram ) )
					{
						PyroliteAmmoUsed += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						PyroliteAmmo -= PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ PowerLevel ];
						if( PyroliteAmmo < 0.0F ) PyroliteAmmo = 0.0F;
					}
				}

				light = FindFreeXLight();
				if( light != (uint16 ) -1 )					// Light attached
				{
					if( !PrimaryLightDetail ) XLights[ light ].Visible = FALSE;
					XLights[ light ].Pos = PrimBulls[i].Pos;
					XLights[ light ].Size = PrimaryWeaponAttribs[ Weapon ].lightsize;
					XLights[ light ].SizeCount = 0.0F;
					XLights[ light ].r = PrimaryWeaponAttribs[ Weapon ].r[ PowerLevel ];
					XLights[ light ].g = PrimaryWeaponAttribs[ Weapon ].g[ PowerLevel ];
					XLights[ light ].b = PrimaryWeaponAttribs[ Weapon ].b[ PowerLevel ];
					XLights[ light ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].light = light;
				}

				fmpoly = FindFreeFmPoly();					// Faceme polygon attached
				if( fmpoly != (uint16 ) -1 )
				{
					FmPolys[ fmpoly ].Pos = PrimBulls[i].Pos;
					FmPolys[ fmpoly ].SeqNum = PrimaryWeaponAttribs[ Weapon ].FmSeq;
					FmPolys[ fmpoly ].Frame = 0.0F;
					FmPolys[ fmpoly ].xsize = ( 16.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].ysize = ( 16.0F * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Frm_Info = PrimaryWeaponAttribs[ Weapon ].FmFrmInfo;
				   	FmPolys[ fmpoly ].LifeCount = 64.0F;
				   	FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				   	FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				   	FmPolys[ fmpoly ].RotSpeed = (float) ( Random_Range( 30 ) + 20 );
#if ACTUAL_TRANS
				   	FmPolys[ fmpoly ].Trans = 128;
#else
				   	FmPolys[ fmpoly ].Trans = 255;
#endif
				   	FmPolys[ fmpoly ].R = 255;
				   	FmPolys[ fmpoly ].G = 255;
				   	FmPolys[ fmpoly ].B = 255;
					FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
					PrimBulls[i].numfmpolys++;
					PrimBulls[i].fmpoly = fmpoly;
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
				}
		   		break;


		   	default:
		   		break;
		}

		if( NetFlag )
		{
			PrimBullPosDirSend( OwnerType, OwnerID, BulletID, Weapon, Group, Pos,
								Offset, Dir, Up, PowerLevel, PLevel );
		}
	}

	return i;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateFireExplosion( VECTOR * Pos, uint16 Group )
{
	uint16	fmpoly;

	if( IsGroupVisible[ Group ] )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].SeqNum = FM_ANIM2;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Exp2_Header;
	   		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
			FmPolys[ fmpoly ].Trans = 128;
#else
			FmPolys[ fmpoly ].Trans = 255;
#endif
	   		FmPolys[ fmpoly ].Dir.x = 0.0F;
	   		FmPolys[ fmpoly ].Dir.y = 0.0F;
	   		FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].R = 255;
			FmPolys[ fmpoly ].G = 255;
			FmPolys[ fmpoly ].B = 255;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			FmPolys[ fmpoly ].RotSpeed = (float) ( Random_Range( 30 ) + 20 );
			FmPolys[ fmpoly ].xsize = NORM_EXPLO_SIZE;
			FmPolys[ fmpoly ].ysize = NORM_EXPLO_SIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	VECTOR	TempUp;
	QUAT	TempQuat;
	MATRIX	TempMat;
	uint16	fmpoly;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		NumSparks = ( Random_Range( 10 ) + 10 );
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			QuatFrom2Vectors( &TempQuat, &Forward, Dir );
		   	QuatToMatrix( &TempQuat, &TempMat );
		   	ApplyMatrix( &TempMat, &SlideUp, &TempUp );
		
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].SeqNum = FM_ARC_RING;
		  	FmPolys[ fmpoly ].Frame = 0.0F;
		  	FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		  	FmPolys[ fmpoly ].Flags = FM_FLAG_DIRCONST;
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		  	FmPolys[ fmpoly ].DirVector = *Dir;
		  	FmPolys[ fmpoly ].UpVector = TempUp;
		   	FmPolys[ fmpoly ].R = 255;
		   	FmPolys[ fmpoly ].G = 32;
		   	FmPolys[ fmpoly ].B = 32;
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = TRANSPULSE_EXP_MINSIZE;
		   	FmPolys[ fmpoly ].ysize = TRANSPULSE_EXP_MINSIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].SeqNum = FM_FLARE;
		  	FmPolys[ fmpoly ].Frame = 0.0F;
		  	FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
		  	FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		   	FmPolys[ fmpoly ].R = 255;
		   	FmPolys[ fmpoly ].G = 255;
		   	FmPolys[ fmpoly ].B = 255;
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = TRANSPULSE_FLARE_SIZE;
		   	FmPolys[ fmpoly ].ysize = TRANSPULSE_FLARE_SIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}

		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 32;
				FmPolys[ fmpoly ].B = 32;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Arc Explosion
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmeArcExplosion( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	VECTOR	TempUp;
	QUAT	TempQuat;
	MATRIX	TempMat;
	uint16	fmpoly;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		NumSparks = ( Random_Range( 10 ) + 10 );
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			QuatFrom2Vectors( &TempQuat, &Forward, Dir );
		   	QuatToMatrix( &TempQuat, &TempMat );
		   	ApplyMatrix( &TempMat, &SlideUp, &TempUp );
		
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].SeqNum = FM_ARC_RING;
		  	FmPolys[ fmpoly ].Frame = 0.0F;
		  	FmPolys[ fmpoly ].Frm_Info = &Trojax_Exp_Header;
		  	FmPolys[ fmpoly ].Flags = FM_FLAG_DIRCONST;
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		  	FmPolys[ fmpoly ].DirVector = *Dir;
		  	FmPolys[ fmpoly ].UpVector = TempUp;
		   	FmPolys[ fmpoly ].R = 32;
		   	FmPolys[ fmpoly ].G = 255;
		   	FmPolys[ fmpoly ].B = 32;
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = TRANSPULSE_EXP_MINSIZE;
		   	FmPolys[ fmpoly ].ysize = TRANSPULSE_EXP_MINSIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
	
	   	fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
		   	FmPolys[ fmpoly ].LifeCount = 48.0F;
		  	FmPolys[ fmpoly ].Pos.x = Pos->x + ( Dir->x * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.y = Pos->y + ( Dir->y * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].Pos.z = Pos->z + ( Dir->z * ( 32.0F * GLOBAL_SCALE ) );
		  	FmPolys[ fmpoly ].SeqNum = FM_FLARE;
		  	FmPolys[ fmpoly ].Frame = 0.0F;
		  	FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
		  	FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
#if ACTUAL_TRANS
		   	FmPolys[ fmpoly ].Trans = 128;
#else
		   	FmPolys[ fmpoly ].Trans = 255;
#endif
		  	FmPolys[ fmpoly ].Dir.x = 0.0F;
		  	FmPolys[ fmpoly ].Dir.y = 0.0F;
		  	FmPolys[ fmpoly ].Dir.z = 0.0F;
		   	FmPolys[ fmpoly ].R = 64;
		   	FmPolys[ fmpoly ].G = 255;
		   	FmPolys[ fmpoly ].B = 64;
		  	FmPolys[ fmpoly ].Speed = 0.0F;
		   	FmPolys[ fmpoly ].Rot = 0.0F;
		   	FmPolys[ fmpoly ].RotSpeed = 0.0F;
		   	FmPolys[ fmpoly ].xsize = TRANSPULSE_FLARE_SIZE;
		   	FmPolys[ fmpoly ].ysize = TRANSPULSE_FLARE_SIZE;
			FmPolys[ fmpoly ].Group = Group;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}

		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 32;
				FmPolys[ fmpoly ].B = 32;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Sparks
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		NumSparks = ( Random_Range( 10 ) + 10 );
	
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 32;
				FmPolys[ fmpoly ].B = 32;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Steam
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSteam( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	int16	NumSparks;
	int16	Count;

	if( IsGroupVisible[ Group ] )
	{
		NumSparks = ( Random_Range( 10 ) + 10 );
	
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
	
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 64;
				FmPolys[ fmpoly ].G = 64;
				FmPolys[ fmpoly ].B = 64;
				FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 4.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE*2;
				FmPolys[ fmpoly ].ysize = SPARKSIZE*2;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Laser Pulse
	Input		:	uint16		Primary weapon number
				:	float		Length of laser
				:	VECTOR	*	Direction
				:	uint16		Group
				:	uint16		EndGroup
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup )
{
	uint16	poly;
	uint16	light;
	uint16	fmpoly;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	int16	NumSparks;
	int16	Count;
	uint16	Range;
	GUNOBJECT * GunPtr;		// the first gun...
	PVSPOTFX * SpotFXPtr;
	QUAT	TempQuat;
	MATRIX	TempMat;

	switch( PrimBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			break;

		case OWNER_ENEMY:
			if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
			{
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			}
			else
			{
				GunPtr = PrimBulls[i].EnemyGun;
				ApplyMatrix( &GunPtr->Mat, &SlideLeft, &LeftVector );
				ApplyMatrix( &GunPtr->Mat, &SlideUp, &UpVector );
			}
			break;

		case OWNER_MODELSPOTFX:
			if( ModelNames[ Models[ PrimBulls[ i ].Owner ].ModelNum ].DoIMorph )
			{
				SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}
			else
			{
				SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}

			if( SpotFXPtr )
			{
				SpotFXPtr += PrimBulls[i].SpotFX;

				ApplyMatrix( &Models[ PrimBulls[ i ].Owner ].Mat, &SpotFXPtr->UpVector, &UpVector );
				CrossProduct( &PrimBulls[i].Dir, &UpVector, &LeftVector );
			}
			else
			{
				QuatFromVector2( Dir, &TempQuat );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			}
			break;

		default:
			QuatFromVector2( Dir, &TempQuat );
			QuatToMatrix( &TempQuat, &TempMat );
			ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			break;
	}

   	light = PrimBulls[ i ].light;
   	if( light != (uint16) -1 )
   	{
   		XLights[ light ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
   		XLights[ light ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
   		XLights[ light ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	}

   	poly = PrimBulls[ i ].poly;
   	if( poly != (uint16) -1 )
   	{
		Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
		Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );

		if( !( ( PrimBulls[i].OwnerType == OWNER_SHIP ) && ( PrimBulls[i].Owner == WhoIAm ) ) )	/* only other people see cross */
		{
		   	poly = Polys[ poly ].Prev;
		   	if( poly != (uint16) -1 )
		   	{
				Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
				Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );
			}
		}
	}

	if( IsGroupVisible[ EndGroup ] )
	{
	   	fmpoly = PrimBulls[ i ].fmpoly;
	   	if( fmpoly != (uint16) -1 )
	   	{
	   		FmPolys[ fmpoly ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
	   		FmPolys[ fmpoly ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
	   		FmPolys[ fmpoly ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
			FmPolys[ fmpoly ].Group = EndGroup;
		}

		Range = (uint16) ( 3.0F * framelag );
		if( Range > 3 ) Range = (uint16) 3;

		NumSparks = Random_Range( Range );
	   
	   	for( Count = 0; Count < NumSparks; Count++ )
	   	{
	   	   	fmpoly = FindFreeFmPoly();
	   
	   	   	if( fmpoly != (uint16 ) -1 )
	   	   	{
	   			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   	   		FmPolys[ fmpoly ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
	   	   		FmPolys[ fmpoly ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
	   	   		FmPolys[ fmpoly ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	   	   		FmPolys[ fmpoly ].Dir = *Dir;
	   	   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
	   	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   	   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
	   			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
	   			FmPolys[ fmpoly ].Trans = 128;
#else
	   			FmPolys[ fmpoly ].Trans = 255;
#endif
	   	   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   	   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   	   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   			FmPolys[ fmpoly ].R = 255;
	   			FmPolys[ fmpoly ].G = 64;
	   			FmPolys[ fmpoly ].B = 64;
	   	   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
	   			FmPolys[ fmpoly ].Rot = 0.0F;
	   			FmPolys[ fmpoly ].xsize = SPARKSIZE;
	   			FmPolys[ fmpoly ].ysize = SPARKSIZE;
	   			FmPolys[ fmpoly ].Group = EndGroup;
	   			NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	   	}
	   		else
	   		{
	   			return;
	   		}
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Laser Pulse
	Input		:	uint16		Primary weapon number
				:	float		Length of laser
				:	VECTOR	*	Direction
				:	uint16		Group
				:	uint16		EndGroup
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmeLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup )
{
	uint16	poly;
	uint16	light;
	uint16	fmpoly;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	int16	NumSparks;
	int16	Count;
	uint16	Range;
	GUNOBJECT * GunPtr;		// the first gun...
	PVSPOTFX * SpotFXPtr;
	QUAT	TempQuat;
	MATRIX	TempMat;

	switch( PrimBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			break;

		case OWNER_ENEMY:
			if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
			{
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			}
			else
			{
				GunPtr = PrimBulls[i].EnemyGun;
				ApplyMatrix( &GunPtr->Mat, &SlideLeft, &LeftVector );
				ApplyMatrix( &GunPtr->Mat, &SlideUp, &UpVector );
			}
			break;

		case OWNER_MODELSPOTFX:
			if( ModelNames[ Models[ PrimBulls[ i ].Owner ].ModelNum ].DoIMorph )
			{
				SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}
			else
			{
				SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}

			if( SpotFXPtr )
			{
				SpotFXPtr += PrimBulls[i].SpotFX;

				ApplyMatrix( &Models[ PrimBulls[ i ].Owner ].Mat, &SpotFXPtr->UpVector, &UpVector );
				CrossProduct( &PrimBulls[i].Dir, &UpVector, &LeftVector );
			}
			else
			{
				QuatFromVector2( Dir, &TempQuat );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			}
			break;

		default:
			QuatFromVector2( Dir, &TempQuat );
			QuatToMatrix( &TempQuat, &TempMat );
			ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			break;
	}

   	light = PrimBulls[ i ].light;
   	if( light != (uint16) -1 )
   	{
   		XLights[ light ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
   		XLights[ light ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
   		XLights[ light ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	}

   	poly = PrimBulls[ i ].poly;
   	if( poly != (uint16) -1 )
   	{
		Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
		Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );

		if( !( ( PrimBulls[i].OwnerType == OWNER_SHIP ) && ( PrimBulls[i].Owner == WhoIAm ) ) )	/* only other people see cross */
		{
		   	poly = Polys[ poly ].Prev;
		   	if( poly != (uint16) -1 )
		   	{
				Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
				Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );
			}
		}
	}

	if( IsGroupVisible[ EndGroup ] )
	{
	   	fmpoly = PrimBulls[ i ].fmpoly;
	   	if( fmpoly != (uint16) -1 )
	   	{
	   		FmPolys[ fmpoly ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
	   		FmPolys[ fmpoly ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
	   		FmPolys[ fmpoly ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
			FmPolys[ fmpoly ].Group = EndGroup;
		}

		Range = (uint16) ( 3.0F * framelag );
		if( Range > 3 ) Range = (uint16) 3;

		NumSparks = Random_Range( Range );
	   
	   	for( Count = 0; Count < NumSparks; Count++ )
	   	{
	   	   	fmpoly = FindFreeFmPoly();
	   
	   	   	if( fmpoly != (uint16 ) -1 )
	   	   	{
	   			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   	   		FmPolys[ fmpoly ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
	   	   		FmPolys[ fmpoly ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
	   	   		FmPolys[ fmpoly ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	   	   		FmPolys[ fmpoly ].Dir = *Dir;
	   	   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
	   	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   	   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
	   			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
	   			FmPolys[ fmpoly ].Trans = 128;
#else
	   			FmPolys[ fmpoly ].Trans = 255;
#endif
	   	   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   	   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   	   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   			FmPolys[ fmpoly ].R = 16;
	   			FmPolys[ fmpoly ].G = 255;
	   			FmPolys[ fmpoly ].B = 16;
	   	   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
	   			FmPolys[ fmpoly ].Rot = 0.0F;
	   			FmPolys[ fmpoly ].xsize = SPARKSIZE;
	   			FmPolys[ fmpoly ].ysize = SPARKSIZE;
	   			FmPolys[ fmpoly ].Group = EndGroup;
	   			NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	   	}
	   		else
	   		{
	   			return;
	   		}
	   	}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Power Laser Pulse
	Input		:	uint16		Primary weapon number
				:	float		Length of laser
				:	VECTOR	*	Direction
				:	uint16		Group
				:	uint16		EndGroup
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmePowerLaserPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup )
{
	uint16	poly;
	uint16	light;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	GUNOBJECT * GunPtr;		// the first gun...
	PVSPOTFX * SpotFXPtr;
	QUAT	TempQuat;
	MATRIX	TempMat;
	VECTOR	EndPos;

	switch( PrimBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			break;

		case OWNER_ENEMY:
			if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
			{
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &UpVector );
			}
			else
			{
				GunPtr = PrimBulls[i].EnemyGun;
				ApplyMatrix( &GunPtr->Mat, &SlideLeft, &LeftVector );
				ApplyMatrix( &GunPtr->Mat, &SlideUp, &UpVector );
			}
			break;

		case OWNER_MODELSPOTFX:
			if( ModelNames[ Models[ PrimBulls[ i ].Owner ].ModelNum ].DoIMorph )
			{
				SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}
			else
			{
				SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}

			if( SpotFXPtr )
			{
				SpotFXPtr += PrimBulls[i].SpotFX;

				ApplyMatrix( &Models[ PrimBulls[ i ].Owner ].Mat, &SpotFXPtr->UpVector, &UpVector );
				CrossProduct( &PrimBulls[i].Dir, &UpVector, &LeftVector );
			}
			else
			{
				QuatFromVector2( Dir, &TempQuat );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			}
			break;

		default:
			QuatFromVector2( Dir, &TempQuat );
			QuatToMatrix( &TempQuat, &TempMat );
			ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			break;
	}

	CreatePowerLaserSparks( &PrimBulls[i].Pos, &PrimBulls[i].Dir, PrimBulls[i].GroupImIn, 64, 64, 192 );

   	light = PrimBulls[ i ].light;
   	if( light != (uint16) -1 )
   	{
   		XLights[ light ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
   		XLights[ light ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
   		XLights[ light ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	}

   	poly = PrimBulls[ i ].poly;
   	if( poly != (uint16) -1 )
   	{
		StartShipScreenShake( ( Polys[ poly ].Frame * 4.0F ) );
		LaserDiameter = ( Polys[ poly ].Frame * ( 32.0F * ( PrimBulls[i].PowerLevel + 1 ) ) );

		EndPos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
		EndPos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
		EndPos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
		CreateScaleExplosion( &EndPos, EndGroup, ( Polys[ poly ].Frame * 3.0F ) );

		Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( LeftVector.x * LaserDiameter );
		Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( LeftVector.y * LaserDiameter );
		Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( LeftVector.z * LaserDiameter );
		Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
		Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
		Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
		Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );

		if( !( ( PrimBulls[i].OwnerType == OWNER_SHIP ) && ( PrimBulls[i].Owner == WhoIAm ) ) )	/* only other people see cross */
		{
		   	poly = Polys[ poly ].Prev;
		   	if( poly != (uint16) -1 )
		   	{
				Polys[ poly ].Pos1.x = PrimBulls[ i ].Pos.x - ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos1.y = PrimBulls[ i ].Pos.y - ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos1.z = PrimBulls[ i ].Pos.z - ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos2.x = PrimBulls[ i ].Pos.x + ( UpVector.x * LaserDiameter );
				Polys[ poly ].Pos2.y = PrimBulls[ i ].Pos.y + ( UpVector.y * LaserDiameter );
				Polys[ poly ].Pos2.z = PrimBulls[ i ].Pos.z + ( UpVector.z * LaserDiameter );
				Polys[ poly ].Pos3.x = Polys[ poly ].Pos2.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos3.y = Polys[ poly ].Pos2.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos3.z = Polys[ poly ].Pos2.z + ( PrimBulls[ i ].Dir.z * Distance );
				Polys[ poly ].Pos4.x = Polys[ poly ].Pos1.x + ( PrimBulls[ i ].Dir.x * Distance );
				Polys[ poly ].Pos4.y = Polys[ poly ].Pos1.y + ( PrimBulls[ i ].Dir.y * Distance );
				Polys[ poly ].Pos4.z = Polys[ poly ].Pos1.z + ( PrimBulls[ i ].Dir.z * Distance );
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Nme Lightning Pulse
	Input		:	uint16		Primary weapon number
				:	float		Length of laser
				:	VECTOR	*	Direction
				:	uint16		Group
				:	uint16		EndGroup
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateNmeLightningPulse( uint16 i, float Distance, VECTOR * Dir, uint16 Group, uint16 EndGroup )
{
	uint16	fmpoly;
	uint16	poly;
	uint16	nextpoly;
	uint16	light;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	VECTOR	NormLeftVector;
	VECTOR	NormUpVector;
	VECTOR	StartPos;
	VECTOR	EndPos;
	int16	NumSegments;
	int16	Count;
	int16	NumSparks;
	float	RandomX;
	float	RandomY;
	float	SubDivision = ( 128 * GLOBAL_SCALE ); //( 1024.0F * GLOBAL_SCALE );
	float	CurrentOffset;
	uint16	XRange = 16;
	uint16	YRange = 16;
	float	LightningDiameter = ( 30.0F * GLOBAL_SCALE );
	float	Frame;
	uint16	Range;
	GUNOBJECT * GunPtr;		// the first gun...
	PVSPOTFX * SpotFXPtr;
	QUAT	TempQuat;
	MATRIX	TempMat;

	if( PrimBulls[i].LifeCount ) Distance = ( 1.0F - ( PrimBulls[i].LifeCount / 12.0F ) ) * Distance;

	switch( PrimBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &NormLeftVector );
			ApplyMatrix( &Ships[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &NormUpVector );
			break;

		case OWNER_ENEMY:
			if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
			{
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideLeft, &NormLeftVector );
				ApplyMatrix( &Enemies[ PrimBulls[ i ].Owner ].Object.FinalMat, &SlideUp, &NormUpVector );
			}
			else
			{
				GunPtr = PrimBulls[i].EnemyGun;
				ApplyMatrix( &GunPtr->Mat, &SlideLeft, &NormLeftVector );
				ApplyMatrix( &GunPtr->Mat, &SlideUp, &NormUpVector );
			}
			break;

		case OWNER_NOBODY:
			NormUpVector = PrimBulls[i].UpVector;
			CrossProduct( &PrimBulls[i].Dir, &PrimBulls[i].UpVector, &NormLeftVector );
			break;

		case OWNER_MODELSPOTFX:
			if( ModelNames[ Models[ PrimBulls[ i ].Owner ].ModelNum ].DoIMorph )
			{
				SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}
			else
			{
				SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[ i ].Owner ].ModelNum ].SpotFX );
			}

			if( SpotFXPtr )
			{
				SpotFXPtr += PrimBulls[i].SpotFX;

				ApplyMatrix( &Models[ PrimBulls[ i ].Owner ].Mat, &SpotFXPtr->UpVector, &NormUpVector );
				CrossProduct( &PrimBulls[i].Dir, &NormUpVector, &NormLeftVector );
			}
			else
			{
				QuatFromVector2( Dir, &TempQuat );
				QuatToMatrix( &TempQuat, &TempMat );
				ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
				ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			}
			break;

		default:
			QuatFromVector2( Dir, &TempQuat );
			QuatToMatrix( &TempQuat, &TempMat );
			ApplyMatrix( &TempMat, &SlideLeft, &LeftVector );
			ApplyMatrix( &TempMat, &SlideUp, &UpVector );
			break;
	}

   	light = PrimBulls[ i ].light;
   	if( light != (uint16) -1 )
   	{
   		XLights[ light ].Pos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
   		XLights[ light ].Pos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
   		XLights[ light ].Pos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
	}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Kill all existing polys
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
   	poly = PrimBulls[ i ].poly;

	for( Count = 0; Count < PrimBulls[i].numpolys; Count++ )
	{
		nextpoly = Polys[ poly ].Prev;
		if( poly != (uint16) -1 ) KillUsedPoly( poly );
		poly = nextpoly;
	}

	PrimBulls[i].numpolys = 0;
	PrimBulls[i].poly = (uint16) -1;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Create new polys
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
	NumSegments = (int16) ceil( Distance / SubDivision );

	if( PrimBulls[i].LifeCount < 4.0F )
	{
		Frame = ( 2.9F - PrimBulls[i].LifeCount );
		if( Frame < 0.0F ) Frame = 0.0F;
	}
	else
	{
		Frame = 0.0F;
	}

	LeftVector.x = ( NormLeftVector.x * LightningDiameter );
	LeftVector.y = ( NormLeftVector.y * LightningDiameter );
	LeftVector.z = ( NormLeftVector.z * LightningDiameter );
	UpVector.x = ( NormUpVector.x * LightningDiameter );
	UpVector.y = ( NormUpVector.y * LightningDiameter );
	UpVector.z = ( NormUpVector.z * LightningDiameter );

	StartPos = PrimBulls[i].Pos;

	for( Count = 0; Count < NumSegments; Count++ )
	{
		CurrentOffset = ( SubDivision * ( Count + 1 ) );

		if( Count != ( NumSegments - 1 ) )
		{
			RandomX = (float) ( Random_Range( XRange ) - ( XRange >> 1 ) );
			RandomY = (float) ( Random_Range( YRange ) - ( YRange >> 1 ) );

			EndPos.x = ( PrimBulls[ i ].Pos.x + ( PrimBulls[ i ].Dir.x * CurrentOffset ) + ( RandomY * NormUpVector.x ) + ( RandomX * NormLeftVector.x ));
			EndPos.y = ( PrimBulls[ i ].Pos.y + ( PrimBulls[ i ].Dir.y * CurrentOffset ) + ( RandomY * NormUpVector.y ) + ( RandomX * NormLeftVector.y ));
			EndPos.z = ( PrimBulls[ i ].Pos.z + ( PrimBulls[ i ].Dir.z * CurrentOffset ) + ( RandomY * NormUpVector.z ) + ( RandomX * NormLeftVector.z ));
		}
		else
		{
			EndPos.x = ( PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Distance ) );
			EndPos.y = ( PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Distance ) );
			EndPos.z = ( PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Distance ) );
		}

		poly = FindFreePoly();
		if( poly != (uint16) -1 )
		{
			Polys[ poly ].Pos1.x = StartPos.x - LeftVector.x;
			Polys[ poly ].Pos1.y = StartPos.y - LeftVector.y;
			Polys[ poly ].Pos1.z = StartPos.z - LeftVector.z;
			Polys[ poly ].Pos2.x = StartPos.x + LeftVector.x;
			Polys[ poly ].Pos2.y = StartPos.y + LeftVector.y;
			Polys[ poly ].Pos2.z = StartPos.z + LeftVector.z;
			Polys[ poly ].Pos3.x = EndPos.x + LeftVector.x;
			Polys[ poly ].Pos3.y = EndPos.y + LeftVector.y;
			Polys[ poly ].Pos3.z = EndPos.z + LeftVector.z;
			Polys[ poly ].Pos4.x = EndPos.x - LeftVector.x;
			Polys[ poly ].Pos4.y = EndPos.y - LeftVector.y;
			Polys[ poly ].Pos4.z = EndPos.z - LeftVector.z;
			Polys[ poly ].Col1.R = 255;
			Polys[ poly ].Col1.G = 255;
			Polys[ poly ].Col1.B = 255;
			Polys[ poly ].Col2.R = 255;
			Polys[ poly ].Col2.G = 255;
			Polys[ poly ].Col2.B = 255;
			Polys[ poly ].Col3.R = 255;
			Polys[ poly ].Col3.G = 255;
			Polys[ poly ].Col3.B = 255;
			Polys[ poly ].Col4.R = 255;
			Polys[ poly ].Col4.G = 255;
			Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
			Polys[ poly ].Trans1 = 128;
			Polys[ poly ].Trans2 = 128;
			Polys[ poly ].Trans3 = 128;
			Polys[ poly ].Trans4 = 128;
#else
			Polys[ poly ].Trans1 = 255;
			Polys[ poly ].Trans2 = 255;
			Polys[ poly ].Trans3 = 255;
			Polys[ poly ].Trans4 = 255;
#endif
			Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
			Polys[ poly ].Frm_Info = &Laser_Header;
			Polys[ poly ].Frame = Frame;
			Polys[ poly ].SeqNum = POLY_NOTHING; //POLY_LASER;
			Polys[ poly ].Group = PrimBulls[i].GroupImIn;
			PrimBulls[i].numpolys++;
			PrimBulls[i].poly = poly;
	
			AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
		}

		poly = FindFreePoly();
		if( poly != (uint16) -1 )
		{
			Polys[ poly ].Pos1.x = StartPos.x - UpVector.x;
			Polys[ poly ].Pos1.y = StartPos.y - UpVector.y;
			Polys[ poly ].Pos1.z = StartPos.z - UpVector.z;
			Polys[ poly ].Pos2.x = StartPos.x + UpVector.x;
			Polys[ poly ].Pos2.y = StartPos.y + UpVector.y;
			Polys[ poly ].Pos2.z = StartPos.z + UpVector.z;
			Polys[ poly ].Pos3.x = EndPos.x + UpVector.x;
			Polys[ poly ].Pos3.y = EndPos.y + UpVector.y;
			Polys[ poly ].Pos3.z = EndPos.z + UpVector.z;
			Polys[ poly ].Pos4.x = EndPos.x - UpVector.x;
			Polys[ poly ].Pos4.y = EndPos.y - UpVector.y;
			Polys[ poly ].Pos4.z = EndPos.z - UpVector.z;
			Polys[ poly ].Col1.R = 255;
			Polys[ poly ].Col1.G = 255;
			Polys[ poly ].Col1.B = 255;
			Polys[ poly ].Col2.R = 255;
			Polys[ poly ].Col2.G = 255;
			Polys[ poly ].Col2.B = 255;
			Polys[ poly ].Col3.R = 255;
			Polys[ poly ].Col3.G = 255;
			Polys[ poly ].Col3.B = 255;
			Polys[ poly ].Col4.R = 255;
			Polys[ poly ].Col4.G = 255;
			Polys[ poly ].Col4.B = 255;
#if ACTUAL_TRANS
			Polys[ poly ].Trans1 = 128;
			Polys[ poly ].Trans2 = 128;
			Polys[ poly ].Trans3 = 128;
			Polys[ poly ].Trans4 = 128;
#else
			Polys[ poly ].Trans1 = 255;
			Polys[ poly ].Trans2 = 255;
			Polys[ poly ].Trans3 = 255;
			Polys[ poly ].Trans4 = 255;
#endif
			Polys[ poly ].Flags = POLY_FLAG_TWOSIDED | POLY_FLAG_DONTCLIP;
			Polys[ poly ].Frm_Info = &Laser_Header;
			Polys[ poly ].Frame = Frame;
			Polys[ poly ].SeqNum = POLY_NOTHING; //POLY_LASER;
			Polys[ poly ].Group = PrimBulls[i].GroupImIn;
			PrimBulls[i].numpolys++;
			PrimBulls[i].poly = poly;
	
			AddPolyToTPage( poly, GetTPage( *Polys[ poly ].Frm_Info, (int16) Polys[ poly ].Frame ) );
		}

		StartPos = EndPos;
	}

   	fmpoly = PrimBulls[ i ].fmpoly;

   	if( fmpoly != (uint16) -1 )
	{
		FmPolys[ fmpoly ].Pos = EndPos;
		FmPolys[ fmpoly ].Group = EndGroup;
	}

	Range = (uint16) ( 3.0F * framelag );
	if( Range > 3 ) Range = (uint16) 3;

	NumSparks = Random_Range( Range );
	
	for( Count = 0; Count < NumSparks; Count++ )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = EndPos;
	   		FmPolys[ fmpoly ].Dir = *Dir;
	   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
   			FmPolys[ fmpoly ].Trans = 128;
#else
   			FmPolys[ fmpoly ].Trans = 255;
#endif
      	   	FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
      	   	FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
      	   	FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
      		FmPolys[ fmpoly ].R = 255;
      		FmPolys[ fmpoly ].G = 16;
      		FmPolys[ fmpoly ].B = 16;
      	   	FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
      		FmPolys[ fmpoly ].Rot = 0.0F;
      		FmPolys[ fmpoly ].xsize = SPARKSIZE;
      		FmPolys[ fmpoly ].ysize = SPARKSIZE;
      		FmPolys[ fmpoly ].Group = EndGroup;
      		NormaliseVector( &FmPolys[ fmpoly ].Dir );
  			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
   	   	}
   		else
   		{
   			return;
  		}
   	}
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Transpulse Trail
	Input		:	uint16		PrimBull Index
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateTransTrail( uint16 i )
{
	uint16	fmpoly;
	int16	Size;
	VECTOR	Right;
	float	Num;

	if( i == (uint16) -1 ) return;
	if( PrimBulls[ i ].numfmpolys == 0 ) return;

	CrossProduct( &PrimBulls[ i ].Dir, &FmPolys[ PrimBulls[i].fmpoly ].DirVector, &Right );

	fmpoly = FindFreeFmPoly();

	if( fmpoly != (uint16 ) -1 )
	{
		Size = (int16) ( ( PrimBulls[ i ].PowerLevel * ARCTRAIL_POWERMUL ) + ARCTRAIL_ADDMIN );					// Max 440;
		Num = ( (float) ( Random_Range( (uint16) ( Size * 2 ) ) - Size ) ) * GLOBAL_SCALE;
		FmPolys[ fmpoly ].LifeCount = 1000.0F;
		FmPolys[ fmpoly ].Pos.x = ( PrimBulls[i].Pos.x + ( Right.x * Num ) );
		FmPolys[ fmpoly ].Pos.y = ( PrimBulls[i].Pos.y + ( Right.y * Num ) );
		FmPolys[ fmpoly ].Pos.z = ( PrimBulls[i].Pos.z + ( Right.z * Num ) );
		FmPolys[ fmpoly ].Dir = PrimBulls[ i ].Dir;
		FmPolys[ fmpoly ].SeqNum = FM_ARC_SPARK;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
		FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
#if ACTUAL_TRANS
		FmPolys[ fmpoly ].Trans = 128;
#else
		FmPolys[ fmpoly ].Trans = 255;
#endif
		FmPolys[ fmpoly ].R = 255;
		FmPolys[ fmpoly ].G = 64;
		FmPolys[ fmpoly ].B = 64;
		FmPolys[ fmpoly ].Speed = ( 16.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].Rot = 0.0F;
		FmPolys[ fmpoly ].xsize = TRANSPULSE_SPARKSIZE;
		FmPolys[ fmpoly ].ysize = TRANSPULSE_SPARKSIZE;
		FmPolys[ fmpoly ].Group = PrimBulls[i].GroupImIn;
		NormaliseVector( &FmPolys[ fmpoly ].Dir );
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Power Level
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DispPowerLevel( void )
{
	int16	Steps, Count;
	int16	x, y;

	x = (d3dappi.szClient.cx / 2 ) - ( 16 * 4 );
	y = d3dappi.szClient.cy - ( d3dappi.szClient.cy / 5 ) + 6;

	switch( Ships[ WhoIAm ].Primary )
	{
		case PULSAR:
		case TROJAX:
		case TRANSPULSE_CANNON:
		case LASER:
			Steps = (int16) ( ( GeneralAmmo + ( MAXGENERALAMMO / 10.0F ) - 1.0F ) / ( MAXGENERALAMMO / 10.0F ) );
			break;

		case SUSS_GUN:
			Steps = (int16) ( ( SussGunAmmo + ( MAXSUSSGUNAMMO / 10.0F ) - 1.0F ) / ( MAXSUSSGUNAMMO / 10.0F ) );
			break;

		case PYROLITE_RIFLE:
			Steps = (int16) ( ( PyroliteAmmo + ( MAXPYROLITEAMMO / 10.0F ) - 1.0F ) / ( MAXPYROLITEAMMO / 10.0F ) );
			break;
	}

	if( PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] )
	{
		if( Steps != 0 )
		{
			if( Steps > 10 ) Steps = 10;

			for( Count = 0; Count < Steps; Count++ )
			{
				PowerString[ Count ] = 'O';
			}
			PowerString[ Steps ] = 0;
			Print4x5Text( &PowerString[ 0 ], x, y, 2 );
		}
		else
		{
			Print4x5Text( "NO AMMO", x, y, 2 );
		}
	
		if( Ships[ WhoIAm ].Primary == LASER )
		{
			Steps = (int16) ( LaserTemperature / 10.0F );

			if( Steps > 10 ) Steps = 10;

			for( Count = 0; Count < Steps; Count++ )
			{
				LaserString[ Count ] = 'O';
			}
			LaserString[ Steps ] = 0;
	
			Print4x5Text( &LaserString[ 0 ], x, y-6, 5 );
		}
	
		if( Ships[ WhoIAm ].Primary == TROJAX && GeneralAmmo != 0.0F && control.fire_primary )
		{
			Steps = (int16) ( PowerLevel / 10.0F );

			if( Steps > 10 ) Steps = 10;

			for( Count = 0; Count < Steps; Count++ )
			{
				PowerString[ Count ] = 'O';
			}
			PowerString[ Steps ] = 0;
	
			Print4x5Text( &PowerString[ 0 ], x, y-6, 1 );
		}
	}
	else
	{
		Print4x5Text( "NO WEAPON", x, y, 2 );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Shield Effect when hit
	Input		:	VECTOR	*	Center of Shield
				:	VECTOR	*	Intersection Point ( Front of shield )
				:	VECTOR	*	Intersection Point2 ( Back of shield )
				:	uint16		ShipHit
				:	int16		Number of Spirals
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateShieldEffect( VECTOR * Pos, VECTOR * IntPos, VECTOR * IntPos2, uint16 ShipHit, int16 Num,
						  uint8 Red, uint8 Green, uint8 Blue )
{
	int16	Count;
	uint16  Poly;
	float	Step;
	float	Angle;
	float	Time;
	float	Val;
	uint8	RVal, GVal, BVal;
	QUAT	StartQuat;
	QUAT	EndQuat;
	VECTOR	TempVector;
	VECTOR	TempVector2;
	MATRIX	TempMat;
	MATRIX	TempInvMat;

	if( IsGroupVisible[ Ships[ ShipHit ].Object.Group ] )
	{
		while( Num-- )
		{
			TempVector.x = ( IntPos->x - Pos->x );
			TempVector.y = ( IntPos->y - Pos->y );
			TempVector.z = ( IntPos->z - Pos->z );
			NormaliseVector( &TempVector );
		
			TempVector2.x = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
			TempVector2.y = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
			TempVector2.z = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
			NormaliseVector( &TempVector2 );
		
			Angle = (float) acos( DotProduct( &TempVector, &TempVector2 ) );
			if( Angle ) Time = D2R( ( 2.0F / Angle ) );
			else Time = 1.0F;
			if( Time > 1.0F ) Time = 1.0F;
		
			QuatFrom2Vectors( &StartQuat, &Forward, &TempVector );
			QuatFrom2Vectors( &EndQuat, &Forward, &TempVector2 );
		
			Step = 0.0F;
		
			for( Count = 0; Count < 4; Count++ )
			{
				if( Step < 1.0F )
				{
					Poly = FindFreePoly();
				
					if( Poly != (uint16) -1 )
					{
						Val = ( 1.0F / ( 256.0F / ( 255.0F - ( ( 3.0F - (float) Count ) * 40.0F ) ) ) );
						RVal = (uint8) ( (float) Red * Val );
						GVal = (uint8) ( (float) Green * Val );
						BVal = (uint8) ( (float) Blue * Val );
	
						Polys[ Poly ].Col1.R = RVal;
						Polys[ Poly ].Col1.G = GVal;
						Polys[ Poly ].Col1.B = BVal;
						Polys[ Poly ].Col2.R = RVal;
						Polys[ Poly ].Col2.G = GVal;
						Polys[ Poly ].Col2.B = BVal;
						Polys[ Poly ].Col3.R = RVal;
						Polys[ Poly ].Col3.G = GVal;
						Polys[ Poly ].Col3.B = BVal;
						Polys[ Poly ].Col4.R = RVal;
						Polys[ Poly ].Col4.G = GVal;
						Polys[ Poly ].Col4.B = BVal;
						Polys[ Poly ].Trans1 = 255;
						Polys[ Poly ].Trans2 = 255;
						Polys[ Poly ].Trans3 = 255;
						Polys[ Poly ].Trans4 = 255;
						Polys[ Poly ].Scale = ( 1.0F - ( ( 3 - Count )  * 0.15F ) );
						Polys[ Poly ].Flags = POLY_FLAG_TWOSIDED;
						Polys[ Poly ].Frm_Info = &Circle_Header;
						Polys[ Poly ].Frame = 0.0F;
						Polys[ Poly ].Quat = StartQuat;
						Polys[ Poly ].Qlerp.start = StartQuat;			/* Starting Quaternion */
						Polys[ Poly ].Qlerp.end = EndQuat;				/* Ending Quaternion */
						Polys[ Poly ].Qlerp.crnt = &Polys[ Poly ].Quat;	/* Address to store new Quaternion */
						Polys[ Poly ].Qlerp.dir = QuatDotProduct( &Polys[ Poly ].Qlerp.start, &Polys[ Poly ].Qlerp.end );
						Polys[ Poly ].Qlerp.time = Step;
						Polys[ Poly ].SeqNum = POLY_SHIELD;
						Polys[ Poly ].Ship = ShipHit;
						Polys[ Poly ].TimeStep = Time;
						Polys[ Poly ].Group = Ships[ ShipHit ].Object.Group;
			
						QuatInterpolate( &Polys[ Poly ].Qlerp );
		
						QuatToMatrix( &Polys[ Poly ].Quat, &TempMat );
						MatrixTranspose( &TempMat, &TempInvMat );
						TempInvMat._41 = Ships[ Polys[ Poly ].Ship ].Object.Pos.x;
						TempInvMat._42 = Ships[ Polys[ Poly ].Ship ].Object.Pos.y;
						TempInvMat._43 = Ships[ Polys[ Poly ].Ship ].Object.Pos.z;
		
						VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 0 ], &Polys[ Poly ].Pos1 );
						VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 1 ], &Polys[ Poly ].Pos2 );
						VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 2 ], &Polys[ Poly ].Pos3 );
						VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 3 ], &Polys[ Poly ].Pos4 );
			
						Step += ( Time * 6.0F );

						AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16) Polys[ Poly ].Frame ) );
					}
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if hit ship
	Input		:	uint16		OwnerType
				:	uint16		Owner
				:	VECTOR	*	Pos
				:	uint16		Group
				:	VECTOR	*	Dir
				:	VECTOR	*	UpDir
				:	float		Length of Ray
				:	VECTOR	*	Int_Point;
				:	VECTOR	*	Int_Point2;
				:	float	*	Dist to Int_Point
				:	float		Weapon Radius;
				:	uint16		Colision Type
	Output		:	uint16		Ship Index ( -1 If none hit )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 CheckHitShip( uint16 OwnerType, uint16 Owner, VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType )
{
	uint16		Count;
	VECTOR		TempVector;
	VECTOR		Int_Temp;
	VECTOR		Int_Temp2;
	float		DistToSphere;
	uint16		ClosestShip;
	float		ClosestLength;
	float		Cos;
	float		ShipRadius;

	ClosestShip = (uint16) -1;
	ClosestLength = *Dist;

	for( Count = 0; Count < MAX_PLAYERS; Count++ )
	{
		if ( (Ships[Count].enable ) && (Ships[Count].Object.Mode != LIMBO_MODE) && ((GameStatus[Count] == STATUS_Normal )||(GameStatus[Count] == STATUS_SinglePlayer ) ) && !( ( OwnerType == OWNER_SHIP ) && ( Count == Owner ) ) )
		{
			if( ( Ships[ Count ].Object.Mode == NORMAL_MODE ) || ( Ships[ Count ].Object.Mode == DEATH_MODE ) )
			{
				if( !SoundInfo[ Ships[ Count ].Object.Group ][ Group ] )
				{
					switch( ColType )
					{
						case COLTYPE_Trojax:
							TempVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );			// Bul - Ship
							TempVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							TempVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
#ifdef OLD_METHOD
							NormaliseVector( &TempVector );
							Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
							ShipRadius = ( SHIP_RADIUS + ( WeaponRadius * Cos ) );
#else
							Cos = DotProduct( &TempVector, Dir );
							if ( Cos >= 0 && Cos <= DirLength )
								ShipRadius = SHIP_RADIUS + WeaponRadius;
							else
								ShipRadius = SHIP_RADIUS;
#endif
							break;
					
						case COLTYPE_Transpulse:
							TempVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );			// Bul - Ship
							TempVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
							TempVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
							NormaliseVector( &TempVector );
							Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
							Cos = (float) ( Cos * ( 1.0F - fabs( DotProduct( &TempVector, UpDir ) ) ) );
							ShipRadius = ( SHIP_RADIUS + ( WeaponRadius * Cos ) );
							break;
					
						case COLTYPE_Sphere:
							ShipRadius = ( SHIP_RADIUS + WeaponRadius );
							break;
					
						case COLTYPE_Point:
							ShipRadius = SHIP_RADIUS;
							break;
					}
					
					if( RaytoSphere2( (VECTOR *) &Ships[ Count ].Object.Pos, SHIP_RADIUS, Pos, Dir,
									  &Int_Temp, &Int_Temp2 ) )
					{
						TempVector.x = ( Int_Temp.x - Pos->x );
						TempVector.y = ( Int_Temp.y - Pos->y );
						TempVector.z = ( Int_Temp.z - Pos->z );
					
						DistToSphere = VectorLength( &TempVector );
					
						if( ( DistToSphere < DirLength ) && ( DistToSphere < ClosestLength ) )
						{
							if( ValidGroupCollision( Pos, Group, &Int_Temp, &Ships[ Count ].Object.Pos, Ships[ Count ].Object.Group ) )
							{
								ClosestLength = DistToSphere;
								ClosestShip = Count;
								*Int_Point = Int_Temp;
								*Int_Point2 = Int_Temp2;
							}
						}
					}
					else
					{
						if( ShipRadius > SHIP_RADIUS )
						{
							if( RaytoSphere2( (VECTOR *) &Ships[ Count ].Object.Pos, ShipRadius, Pos, Dir,
											  &Int_Temp, &Int_Temp2 ) )
							{
								TempVector.x = ( Int_Temp.x - Pos->x );
								TempVector.y = ( Int_Temp.y - Pos->y );
								TempVector.z = ( Int_Temp.z - Pos->z );
							
								DistToSphere = VectorLength( &TempVector );
							
								if( ( DistToSphere < DirLength ) && ( DistToSphere < ClosestLength ) )
								{
									if( ValidGroupCollision( Pos, Group, &Int_Temp, &Ships[ Count ].Object.Pos, Ships[ Count ].Object.Group ) )
									{
										ClosestLength = DistToSphere;
										ClosestShip = Count;
										*Int_Point = Int_Temp;
										*Int_Point2 = Int_Temp2;
									}
								}
							}
							else
							{
								if( !Ships[ Count ].Invul )
								{
									if( PointToSphere( (VERT *) &Ships[ Count ].Object.Pos, ShipRadius, (VERT *) Pos ) )
									{
										TempVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
										TempVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
										TempVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
										DistToSphere = VectorLength( &TempVector );

										ClosestLength = DistToSphere;
										ClosestShip = Count;
										*Int_Point = *Pos;
										*Int_Point2 = *Pos;
									}
								}
							}
						}
						else
						{
							if( !Ships[ Count ].Invul )
							{
								if( PointToSphere( (VERT *) &Ships[ Count ].Object.Pos, ShipRadius, (VERT *) Pos ) )
								{
									TempVector.x = ( Ships[ Count ].Object.Pos.x - Pos->x );
									TempVector.y = ( Ships[ Count ].Object.Pos.y - Pos->y );
									TempVector.z = ( Ships[ Count ].Object.Pos.z - Pos->z );
									DistToSphere = VectorLength( &TempVector );

									ClosestLength = DistToSphere;
									ClosestShip = Count;
									*Int_Point = *Pos;
									*Int_Point2 = *Pos;
								}
							}
						}
					}
				}
			}
		}
	}

	if( ClosestShip != (uint16) -1 ) *Dist = ClosestLength;

	return( ClosestShip );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if hit secondary mine/missile
	Input		:	VECTOR	*	Pos
				:	uint16		Group
				:	VECTOR	*	Dir
				:	VECTOR	*	UpDir
				:	float		Length of Ray
				:	VECTOR	*	Int_Point;
				:	VECTOR	*	Int_Point2;
				:	float	*	Dist to Int_Point
				:	float		Weapon Radius
				:	uint16		Collison Type
	Output		:	uint16		SecBull Index ( -1 If none hit )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 CheckHitSecondary( VECTOR * Pos, uint16 Group, VECTOR * Dir, VECTOR * UpDir, float DirLength, VECTOR * Int_Point, VECTOR * Int_Point2, float * Dist, float WeaponRadius, uint16 ColType )
{
	VECTOR		Int_Temp;
	VECTOR		Int_Temp2;
	VECTOR		TempVector;
	float		DistToSphere;
	uint16		ClosestSecBull;
	float		ClosestLength;
	float		SecBullRadius;
	float		Cos;
	int16		Count;
	GROUPLIST * GroupsVisible;
	uint16	  * GroupList;
	uint16		CurrentGroup;
	SECONDARYWEAPONBULLET * SecBull;
	uint16		DebugCount = 0;

	ClosestSecBull = (uint16) -1;
	ClosestLength = *Dist;

	GroupsVisible = VisibleGroups( Group );

	GroupList = GroupsVisible->group;

	for( Count = 0; Count < GroupsVisible->groups; Count++ )
	{
		CurrentGroup = GroupList[ Count ];

		SecBull = SecBullGroups[ CurrentGroup ];

		while( SecBull )
		{
			if( DebugCount > MAXSECONDARYWEAPONBULLETS )
			{
				Msg( "CheckHitSecondary() Link list corrupt!!" );
				return( (uint16) -1 );
			}

			DebugCount++;

			if( SecBull->Flags & SECFLAGS_CanShoot )
			{
				switch( ColType )
				{
					case COLTYPE_Trojax:
						TempVector.x = ( SecBull->Pos.x - Pos->x );			// Bul - Ship
						TempVector.y = ( SecBull->Pos.y - Pos->y );
						TempVector.z = ( SecBull->Pos.z - Pos->z );
						NormaliseVector( &TempVector );
						Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
						SecBullRadius = ( MINE_RADIUS + ( WeaponRadius * Cos ) );
						break;
				
					case COLTYPE_Transpulse:
						TempVector.x = ( SecBull->Pos.x - Pos->x );			// Bul - Ship
						TempVector.y = ( SecBull->Pos.y - Pos->y );
						TempVector.z = ( SecBull->Pos.z - Pos->z );
						NormaliseVector( &TempVector );
						Cos = (float) ( 1.0F - fabs( DotProduct( &TempVector, Dir ) ) );
						Cos = (float) ( Cos * ( 1.0F - fabs( DotProduct( &TempVector, UpDir ) ) ) );
						SecBullRadius = ( MINE_RADIUS + ( WeaponRadius * Cos ) );
						break;
				
					case COLTYPE_Sphere:
						SecBullRadius = ( MINE_RADIUS + WeaponRadius );
						break;
				
					case COLTYPE_Point:
						SecBullRadius = MINE_RADIUS;
						break;
				}
				
				
				if( RaytoSphere2( &SecBull->Pos, SecBullRadius, Pos, Dir,
								  &Int_Temp, &Int_Temp2 ) )
				{
					TempVector.x = ( Int_Temp.x - Pos->x );
					TempVector.y = ( Int_Temp.y - Pos->y );
					TempVector.z = ( Int_Temp.z - Pos->z );
				
					DistToSphere = VectorLength( &TempVector );
				
					if( ( DistToSphere <= DirLength ) && ( DistToSphere < ClosestLength ) )
					{
						if( ValidGroupCollision( Pos, Group, &Int_Temp, &SecBull->Pos, SecBull->GroupImIn ) )
						{
							ClosestLength = DistToSphere;
							ClosestSecBull = SecBull->Index;
							*Int_Point = Int_Temp;
							*Int_Point2 = Int_Temp2;
						}
					}
				}
			}

			SecBull = SecBull->NextInGroup;
		}
	}

	if( ClosestSecBull != (uint16) -1 ) *Dist = ClosestLength;

	return( ClosestSecBull );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check valid group ray collision
	Input		:	VECTOR	*	StartPos
				:	uint16		StartGroup
				:	VECTOR	*	IntPoint
				:	VECTOR	*	Object Pos
				:	uint16		Object Group
	Output		:	BOOL		True/False = Valid/Not
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ValidGroupCollision( VECTOR * StartPos, uint16 StartGroup, VECTOR * IntPoint, VECTOR * ObjPos, uint16 ObjGroup )
{
	uint16	Group;
	VECTOR	MoveOff;

	MoveOff.x = ( IntPoint->x - StartPos->x );
	MoveOff.y = ( IntPoint->y - StartPos->y );
	MoveOff.z = ( IntPoint->z - StartPos->z );

	Group = MoveGroup( &Mloadheader, StartPos, StartGroup, &MoveOff );

	MoveOff.x = ( ObjPos->x - IntPoint->x );
	MoveOff.y = ( ObjPos->y - IntPoint->y );
	MoveOff.z = ( ObjPos->z - IntPoint->z );

	Group = MoveGroup( &Mloadheader, IntPoint, Group, &MoveOff );

	if( Group != ObjGroup ) return( FALSE );

	return( TRUE );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Ammo level control
	Input		:	Check ammo levels and do appropriate changed
	Output		:	void
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void PrimarySelectControl( void )
{
	float	AmmoLevels[ MAXPRIMARYWEAPONS ];
	int16	Count;

	AmmoLevels[ PULSAR ] = GeneralAmmo;
	AmmoLevels[ TROJAX ] = GeneralAmmo;
	AmmoLevels[ PYROLITE_RIFLE ] = PyroliteAmmo;
	AmmoLevels[ TRANSPULSE_CANNON ] = GeneralAmmo;
	AmmoLevels[ SUSS_GUN ] = SussGunAmmo;
	AmmoLevels[ LASER ] = GeneralAmmo;

	if( !AmmoLevels[ Ships[ WhoIAm ].Primary ] || !PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] )
	{
		for( Count = 0; Count < MAXPRIMARYWEAPONS; Count++ )
		{
			if( PrimaryWeaponsGot[ player_config->primary_order[ Count ] ] )
			{
				if( AmmoLevels[ player_config->primary_order[ Count ] ] )
				{
					Ships[ WhoIAm ].Primary = (BYTE) player_config->primary_order[ Count ];
					return;
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create non intersection Line for debugging
	Input		:	VECTOR	*	Pos
				:	VECTOR	*	Dir
				:	uint16		Group
				:	int8		Red
				:	int8		Green
				:	int8		Blue
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateDebugLine( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue )
{
	uint16	line;
	VECTOR	TempDir;

	line = FindFreeLine();						// Line attached
   	if( line != (uint16 ) -1 )
   	{
		TempDir = *Dir;
		NormaliseVector( &TempDir );
   		Lines[ line ].StartPos = *Pos;
   		Lines[ line ].EndPos.x = ( Pos->x + ( TempDir.x * MaxColDistance ) );
   		Lines[ line ].EndPos.y = ( Pos->y + ( TempDir.y * MaxColDistance ) );
   		Lines[ line ].EndPos.z = ( Pos->z + ( TempDir.z * MaxColDistance ) );
   		Lines[ line ].StartCol.R = 255;
   		Lines[ line ].StartCol.G = 255;
   		Lines[ line ].StartCol.B = 255;
   		Lines[ line ].EndCol.R = Red;
   		Lines[ line ].EndCol.G = Green;
   		Lines[ line ].EndCol.B = Blue;
   		Lines[ line ].StartTrans = 255;
   		Lines[ line ].EndTrans = 255;
   		Lines[ line ].Group = Group;
   	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Enable All Primary Lights
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void EnablePrimaryLights( void )
{
	uint16	i;

	i = FirstPrimBullUsed;

	while( i != (uint16) -1 )
	{
		if( PrimBulls[ i ].light != (uint16) -1 )
		{
			XLights[ PrimBulls[ i ].light ].Visible = TRUE;
		}
 		
		i = PrimBulls[ i ].Prev;						/* Next Primary Bull */
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Disable All Primary Lights
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisablePrimaryLights( void )
{
	uint16	i;

	i = FirstPrimBullUsed;

	while( i != (uint16) -1 )
	{
		if( PrimBulls[ i ].light != (uint16) -1 )
		{
			XLights[ PrimBulls[ i ].light ].Visible = FALSE;
		}
 		
		i = PrimBulls[ i ].Prev;						/* Next Primary Bull */
	}																				
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fire primary weapons of other players
	Input		:	uint16		Ship Number
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FirePrimaryWeapons( uint8 Ship )
{
	int16	Count;
	VECTOR	Pos;
	VECTOR	Recoil;
	VECTOR	Dir;
	VECTOR	DownVector;
	VECTOR	LeftVector;
	VECTOR	UpVector;
	VECTOR	DirVector;
	VECTOR	Rotation;
	uint16	ID;
	int16	Toggle;
	float	PowerLevel;
	uint16	Model;
	float	Speed;
	QUAT	StepQuat;
	int8	Weapon;
	uint16	i;
	float	FrmLagAdd;

	FrmLagAdd = GlobalFramelagAddition;

	Weapon = Ships[ Ship ].Primary;

	PowerLevel = Ships[Ship].PrimPowerLevel;
	if( ( Ships[Ship].Object.Flags & SHIP_PrimToggle ) ) Toggle = 1;
	else Toggle = 0;
	ID = Ships[Ship].PrimID;

	MakeQuat( 0.0F , 0.0F , Ships[Ship].Object.Bank, &StepQuat );
	QuatMultiply(  &Ships[Ship].Object.Quat , &StepQuat , &Ships[Ship].Object.FinalQuat );
	QuatToMatrix( &Ships[Ship].Object.FinalQuat, &Ships[Ship].Object.FinalMat );

	ApplyMatrix( &Ships[Ship].Object.FinalMat, &Forward, &Dir );
	ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideDown, &DownVector );	/* Calc Down Vector */
	ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */

	if( ( Ships[ Ship ].Object.Flags & SHIP_MulFire ) )
	{
		ID -= Ships[ Ship ].NumMultiples;

		for( Count = 0; Count < Ships[ Ship ].NumMultiples; Count++ )
		{
			Model = Ships[ Ship ].OrbModels[ Count ];
			
			if( Model != (uint16) -1 )
			{
				Rotation.x = (float) sin( D2R( Models[ Model ].AxisRot ) );
				Rotation.y = (float) cos( D2R( Models[ Model ].AxisRot ) );
				Rotation.z = 0.0F;
				ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &Rotation, &UpVector );	// Calc Up Vector
				ApplyMatrix( &Ships[ Ship ].Object.FinalMat, &Forward, &DirVector );	// Calc Dir Vector
				Pos.x = ( UpVector.x * ORBIT_RADIUS );
				Pos.y = ( UpVector.y * ORBIT_RADIUS );
				Pos.z = ( UpVector.z * ORBIT_RADIUS );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, ORBITPULSAR,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &DirVector, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
			}
			ID++;
		}
	}

	ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideUp, &UpVector );		/* Calc Up Vector */

	if( ( Ships[ Ship ].Object.Flags & SHIP_PrimFire ) )
	{
		switch( Weapon )
		{
			case PULSAR:
				if( Ships[ Ship ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) Ships[ Ship ].Object.Flags |= SHIP_PrimToggle;

				if( ( Toggle ) || ( Ships[ Ship ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) )
				{
					Pos.x = ( LeftVector.x * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
					Pos.y = ( LeftVector.y * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
					Pos.z = ( LeftVector.z * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
					i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
										Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
										Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
					if( i != (uint16) -1 )
					{
						PrimBulls[i].FramelagAddition = FrmLagAdd;
					}
				}

				if( Ships[ Ship ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) Ships[ Ship ].Object.Flags &= ~SHIP_PrimToggle;

				if( ( !Toggle ) || ( Ships[ Ship ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL ) )
				{
					Pos.x = -(LeftVector.x * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
					Pos.y = -(LeftVector.y * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
					Pos.z = -(LeftVector.z * ( 100.0F * GLOBAL_SCALE ) )+(DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
					i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + 1 ), Weapon,
										Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
										Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
					if( i != (uint16) -1 )
					{
						PrimBulls[i].FramelagAddition = FrmLagAdd;
					}
				}
				break;
		
			case TROJAX:								/* Fire on release or when max reached */
				ApplyMatrix( &Ships[Ship].Object.FinalMat, &Forward, &Dir );
				ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideDown, &DownVector );		/* Calc Up Vector */
	   			ApplyMatrix( &Ships[Ship].Object.FinalMat, &SlideLeft, &LeftVector );	/* Calc Left Vector */
	
				if( TrojaxOwnersID[ Ship ] )
				{
					StopSfx( TrojaxOwnersID[ Ship ] ); 
				}
	
				Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
				Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
				Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
		
				Recoil.x = 0.0F;
				Recoil.y = 0.0F;
				Recoil.z = -( ( PowerLevel / 5.0F ) * GLOBAL_SCALE );
				ForceInternalOneOff( Ship, &Recoil );
				break;
		
			case PYROLITE_RIFLE:
				Pos.x = ( DownVector.x * ( 100.0F * GLOBAL_SCALE ) );
				Pos.y = ( DownVector.y * ( 100.0F * GLOBAL_SCALE ) );
				Pos.z = ( DownVector.z * ( 100.0F * GLOBAL_SCALE ) );

				PyroLightFlag = TRUE;

				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
#if 1
				Speed = (float) PrimaryWeaponAttribs[ Weapon ].FireDelay[ Ships[ Ship ].Object.PowerLevel ] * framelag;
				Speed *= ( ( PrimaryWeaponAttribs[ Weapon ].Speed[ Ships[ Ship ].Object.PowerLevel ] ) * 0.33333F );
				Pos.x += ( Dir.x * Speed );
				Pos.y += ( Dir.y * Speed );
				Pos.z += ( Dir.z * Speed );
				i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + 1 ), Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}

				Pos.x += ( Dir.x * Speed );
				Pos.y += ( Dir.y * Speed );
				Pos.z += ( Dir.z * Speed );
				i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + 2 ), Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
#endif
				break;
		
			case TRANSPULSE_CANNON:
				Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
				Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
				Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
				break;
		
			case SUSS_GUN:
				Pos.x = ( DownVector.x * ( 75.0F * GLOBAL_SCALE ) );
				Pos.y = ( DownVector.y * ( 75.0F * GLOBAL_SCALE ) );
				Pos.z = ( DownVector.z * ( 75.0F * GLOBAL_SCALE ) );

				for( Count = 0; Count < 4; Count++ )
				{
					i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + Count ), Weapon,
										Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
										Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
					if( i != (uint16) -1 )
					{
						PrimBulls[i].FramelagAddition = FrmLagAdd;
					}
				}
				break;
		
			case LASER:
				Pos.x = ( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				Pos.y = (    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
				Pos.z = (    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
	
				Pos.x = -( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				Pos.y = -(    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
				Pos.z = -(    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + 1 ), Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
				break;

			case NME_LIGHTNING:
				Pos.x = ( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				Pos.y = (    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
				Pos.z = (    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
	
				Pos.x = -( -100.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				Pos.y = -(    0.0F * GLOBAL_SCALE ) + ( -75.0F * GLOBAL_SCALE );
				Pos.z = -(    0.0F * GLOBAL_SCALE ) + (   0.0F * GLOBAL_SCALE );
				i = InitOnePrimBull( OWNER_SHIP, Ship, (uint16) ( ID + 1 ), Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
				break;

			case NME_POWERLASER:
				Pos.x = (   0.0F * GLOBAL_SCALE );
				Pos.y = ( -75.0F * GLOBAL_SCALE );
				Pos.z = (   0.0F * GLOBAL_SCALE );
				i = InitOnePrimBull( OWNER_SHIP, Ship, ID, Weapon,
									Ships[ Ship ].Object.Group, &Ships[ Ship ].Object.Pos, &Pos, &Dir, &UpVector,
									Ships[ Ship ].Object.PowerLevel, PowerLevel, FALSE );
				if( i != (uint16) -1 )
				{
					PrimBulls[i].FramelagAddition = FrmLagAdd;
				}
				break;
		
			default:
				break;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Collision Sparks
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	int16	NumSparks;
	int16	Count;
	float	Scale;

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );

	if( IsGroupVisible[ Group ] )
	{
		CreateBigFlash( Pos, Group, 3.5F );
	
		fmpoly = FindFreeFmPoly();
	
		if( fmpoly != (uint16 ) -1 )
		{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
			FmPolys[ fmpoly ].Pos = *Pos;
			FmPolys[ fmpoly ].SeqNum = FM_ANIM;
			FmPolys[ fmpoly ].Frame = 0.0F;
			FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
			FmPolys[ fmpoly ].R = 192;
			FmPolys[ fmpoly ].G = 192;
			FmPolys[ fmpoly ].B = 192;
			FmPolys[ fmpoly ].Dir.x = 0.0F;
			FmPolys[ fmpoly ].Dir.y = 0.0F;
			FmPolys[ fmpoly ].Dir.z = 0.0F;
			FmPolys[ fmpoly ].Speed = 0.0F;
			FmPolys[ fmpoly ].Group = Group;
			FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
			Scale = (float) ( Random_Range( 5 ) + 5 ) * GLOBAL_SCALE;
			FmPolys[ fmpoly ].xsize = Scale;
			FmPolys[ fmpoly ].ysize = Scale;
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		}

		NumSparks = ( Random_Range( 2 ) + 1 );
		
		for( Count = 0; Count < NumSparks; Count++ )
		{
		   	fmpoly = FindFreeFmPoly();
		
		   	if( fmpoly != (uint16 ) -1 )
		   	{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
		   		FmPolys[ fmpoly ].Pos = *Pos;
		   		FmPolys[ fmpoly ].Dir = *Dir;
		   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
		   		FmPolys[ fmpoly ].Frame = 0.0F;
		   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
				FmPolys[ fmpoly ].Trans = 255;
		   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
		   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
				FmPolys[ fmpoly ].R = 255;
				FmPolys[ fmpoly ].G = 64;
				FmPolys[ fmpoly ].B = 64;
		   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].Rot = 0.0F;
				FmPolys[ fmpoly ].xsize = SPARKSIZE;
				FmPolys[ fmpoly ].ysize = SPARKSIZE;
				FmPolys[ fmpoly ].Group = Group;
				NormaliseVector( &FmPolys[ fmpoly ].Dir );
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
		   	}
			else
			{
				return;
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Collision Sparks
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateSmallColSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group )
{
	uint16	fmpoly;
	float	Scale;
	int16	NumSparks;
	int16	Count;

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );

	if( IsGroupVisible[ Group ] )
	{
		if( !Random_Range( 5 ) )
		{
			CreateBigFlash( Pos, Group, 3.5F );
	
			fmpoly = FindFreeFmPoly();
			if( fmpoly != (uint16 ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
				FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 128 ) ) - 64.0F ) * GLOBAL_SCALE );
				FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
				Scale = (float) ( Random_Range( 5 ) + 5 ) * GLOBAL_SCALE;
				FmPolys[ fmpoly ].xsize = Scale;
				FmPolys[ fmpoly ].ysize = Scale;
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
		
			NumSparks = ( Random_Range( 2 ) + 1 );
			
			for( Count = 0; Count < NumSparks; Count++ )
			{
			   	fmpoly = FindFreeFmPoly();
			
			   	if( fmpoly != (uint16 ) -1 )
			   	{
					FmPolys[ fmpoly ].LifeCount = 1000.0F;
			   		FmPolys[ fmpoly ].Pos = *Pos;
			   		FmPolys[ fmpoly ].Dir = *Dir;
			   		FmPolys[ fmpoly ].SeqNum = FM_BITS;
			   		FmPolys[ fmpoly ].Frame = 0.0F;
			   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
					FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
					FmPolys[ fmpoly ].Trans = 255;
			   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
					FmPolys[ fmpoly ].R = 255;
					FmPolys[ fmpoly ].G = 64;
					FmPolys[ fmpoly ].B = 64;
			   		FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 8.0F ) * GLOBAL_SCALE;
					FmPolys[ fmpoly ].Rot = 0.0F;
					FmPolys[ fmpoly ].xsize = SPARKSIZE;
					FmPolys[ fmpoly ].ysize = SPARKSIZE;
					FmPolys[ fmpoly ].Group = Group;
					NormaliseVector( &FmPolys[ fmpoly ].Dir );
					AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			   	}
				else
				{
					return;
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if any other ships charging
	Input		:	uint16		Ship
				:	uint16		Old Flags
				:	uint16		New Flags
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CheckCharging( uint16 Ship, uint32 OldFlags, uint32 NewFlags )
{
	float Freq;

	if( ( !( OldFlags & SHIP_Charging ) ) && ( NewFlags & SHIP_Charging ) )
	{
		Freq = TrojaxFreqTable[ Ships[ Ship ].Object.PowerLevel ];
		TrojaxOwnersID[ Ship ] = PlayPannedSfx( SFX_Trojax_Charge, Ships[ Ship ].Object.Group , &Ships[ Ship ].Object.Pos, Freq );
	}
}

int16	CopyOfSecondaryAmmo[ MAXSECONDARYWEAPONS ];
int16	CopyOfSecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
int16	CopyOfWeaponsGot[ MAXPRIMARYWEAPONS ];
BYTE	CopyOfPrimary = PULSAR;
BYTE	CopyOfSecondary = MUGMISSILE;
int16	CopyOfPowerLevel = 0;
float	CopyOfGeneralAmmo = 0.0F;
float	CopyOfSussGunAmmo = 0.0F;
float	CopyOfPyroliteAmmo = 0.0F;
float	CopyOfNitroFuel = 0.0F;
BYTE	CopyOfOrbitals = 0;
float	CopyOfOrbAmmo[ MAXMULTIPLES ];

int16	GodModeCopyOfSecondaryAmmo[ MAXSECONDARYWEAPONS ];
int16	GodModeCopyOfSecondaryWeaponsGot[ MAXSECONDARYWEAPONS ];
int16	GodModeCopyOfWeaponsGot[ MAXPRIMARYWEAPONS ];
BYTE	GodModeCopyOfPrimary = PULSAR;
BYTE	GodModeCopyOfSecondary = MUGMISSILE;
int16	GodModeCopyOfPowerLevel = 0;
float	GodModeCopyOfGeneralAmmo = 0.0F;
float	GodModeCopyOfSussGunAmmo = 0.0F;
float	GodModeCopyOfPyroliteAmmo = 0.0F;
float	GodModeCopyOfNitroFuel = 0.0F;
BYTE	GodModeCopyOfOrbitals = 0;
float	GodModeCopyOfOrbAmmo[ MAXMULTIPLES ];

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Give me all weapons
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GivemeAllWeapons( void )
{
	int16	Count;

	if( ( WhoIAm < 0 ) || ( WhoIAm >= ( MAX_PLAYERS + 1 ) ) ) return;

	for( Count = 0; Count < MAXPRIMARYWEAPONS; Count++ ) GodModeCopyOfWeaponsGot[ Count ] = PrimaryWeaponsGot[ Count ];
	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		GodModeCopyOfSecondaryAmmo[ Count ] = SecondaryAmmo[ Count ];
		GodModeCopyOfSecondaryWeaponsGot[ Count ] = SecondaryWeaponsGot[ Count ];
	}

	GodModeCopyOfPrimary = Ships[ WhoIAm ].Primary;
	GodModeCopyOfSecondary = Ships[ WhoIAm ].Secondary;
	GodModeCopyOfPowerLevel = Ships[ WhoIAm ].Object.PowerLevel;
	GodModeCopyOfGeneralAmmo = GeneralAmmo;
	GodModeCopyOfSussGunAmmo = SussGunAmmo;
	GodModeCopyOfPyroliteAmmo = PyroliteAmmo;
	GodModeCopyOfNitroFuel = NitroFuel;

	PrimaryWeaponsGot[ PULSAR ] = 1;
	PrimaryWeaponsGot[ TROJAX ] = 1;
	PrimaryWeaponsGot[ PYROLITE_RIFLE ] = 1;
	PrimaryWeaponsGot[ TRANSPULSE_CANNON ] = 1;
	PrimaryWeaponsGot[ SUSS_GUN ] = 1;
	PrimaryWeaponsGot[ LASER ] = 1;
	Ships[ WhoIAm ].Primary = PULSAR;
	Ships[ WhoIAm ].Secondary = MUGMISSILE;
	Ships[ WhoIAm ].Object.PowerLevel = ( MAXPOWERLEVELS - 1 );
	PyroliteAmmo = MAXPYROLITEAMMO;
	SussGunAmmo = MAXSUSSGUNAMMO;
	GeneralAmmo = MAXGENERALAMMO;
	NitroFuel = 100.0F;

	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecondaryAmmo[ Count ] = 1;
		SecondaryWeaponsGot[ Count ] = 1;
	}

	if( Ships[ WhoIAm ].Invul && PickupInvulnerability ) NumInvuls++;

	Ships[ WhoIAm ].InvulTimer = INVULNERABILITY_TIME;
	Ships[ WhoIAm ].Invul = TRUE;			
	PickupInvulnerability = FALSE;

	GivemeSuperNashram();
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Lose all weapons
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void LoseAllWeapons( void )
{
	int16	Count;

	LoseSuperNashram();

	if( ( WhoIAm < 0 ) || ( WhoIAm >= ( MAX_PLAYERS + 1 ) ) ) return;

	for( Count = 0; Count < MAXPRIMARYWEAPONS; Count++ ) PrimaryWeaponsGot[ Count ] = GodModeCopyOfWeaponsGot[ Count ];
	for( Count = 0; Count < MAXSECONDARYWEAPONS; Count++ )
	{
		SecondaryAmmo[ Count ] = GodModeCopyOfSecondaryAmmo[ Count ];
		SecondaryWeaponsGot[ Count ] = GodModeCopyOfSecondaryWeaponsGot[ Count ];
	}

	Ships[ WhoIAm ].Primary = GodModeCopyOfPrimary;
	Ships[ WhoIAm ].Secondary = GodModeCopyOfSecondary;
	Ships[ WhoIAm ].Object.PowerLevel = GodModeCopyOfPowerLevel;
	GeneralAmmo = GodModeCopyOfGeneralAmmo;
	SussGunAmmo = GodModeCopyOfSussGunAmmo;
	PyroliteAmmo = GodModeCopyOfPyroliteAmmo;
	NitroFuel = GodModeCopyOfNitroFuel;

	Ships[ WhoIAm ].InvulTimer = 0.0F;
	Ships[ WhoIAm ].Invul = FALSE;			
	PickupInvulnerability = FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop Currently selected primary weapon
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DropPrimary( void )
{
	float	Ammo;
	float	Speed;
	float	LifeCount;
	int16	PickupNum;
	VECTOR	DirVector;

	switch( Ships[ WhoIAm ].Primary )
	{
		case PULSAR:
		case TROJAX:
		case TRANSPULSE_CANNON:
		case LASER:
			Ammo = GeneralAmmo;
			break;
		case PYROLITE_RIFLE:
			Ammo = PyroliteAmmo;
			break;
		case SUSS_GUN:
			Ammo = SussGunAmmo;
			break;
	}

	if( ( Ships[ WhoIAm ].Primary != PULSAR ) && ( Ammo ) )
	{
		if( PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] )
		{
			ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &DirVector );
			PickupNum = ( PICKUP_Trojax + ( Ships[ WhoIAm ].Primary - 1 ) );
			LifeCount = -1.0F;
			Speed = PICKUP_SPEED;
			InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
			PrimaryWeaponsGot[ Ships[ WhoIAm ].Primary ] = 0;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Return method of death string
	Input		:	BYTE		Weapon Type
				:	BYTE		Weapon
				:	char	*	Dest String
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetDeathString( BYTE WeaponType, BYTE Weapon, char * DstString )
{
	switch( WeaponType )
	{
		case WEPTYPE_Primary:
			switch( Weapon )
			{
				case PULSAR:
					sprintf( DstString, LT_DEATHMETHOD_PULSAR );
					break;
				case TROJAX:
					sprintf( DstString, LT_DEATHMETHOD_TROJAX );
					break;
				case PYROLITE_RIFLE:
					sprintf( DstString, LT_DEATHMETHOD_PYROLITE );
					break;
				case TRANSPULSE_CANNON:
					sprintf( DstString, LT_DEATHMETHOD_TRANSPULSE );
					break;
				case SUSS_GUN:
					sprintf( DstString, LT_DEATHMETHOD_SUSS_GUN );
					break;
				case LASER:
					sprintf( DstString, LT_DEATHMETHOD_LASER );
					break;
				case ORBITPULSAR:
					sprintf( DstString, LT_DEATHMETHOD_ORBITOR );
					break;
				case NME_BULLET1:
					sprintf( DstString, LT_DEATHMETHOD_NME_BULLET1 );
					break;
				case NME_PULSAR:
					sprintf( DstString, LT_DEATHMETHOD_NME_PULSAR );
					break;
				case NME_TROJAX:
					sprintf( DstString, LT_DEATHMETHOD_NME_TROJAX );
					break;
				case NME_PYROLITE:
					sprintf( DstString, LT_DEATHMETHOD_NME_PYROLITE );
					break;
				case NME_TRANSPULSE:
					sprintf( DstString, LT_DEATHMETHOD_NME_TRANSPULSE );
					break;
				case NME_SUSS_GUN:
					sprintf( DstString, LT_DEATHMETHOD_NME_SUSS_GUN );
					break;
				case NME_LASER:
					sprintf( DstString, LT_DEATHMETHOD_NME_LASER );
					break;
				case NME_LIGHTNING:
					sprintf( DstString, LT_DEATHMETHOD_NME_LIGHTNING );
					break;
				case FLAMES:
					sprintf( DstString, LT_DEATHMETHOD_FLAMES );
					break;
				case NME_POWERLASER:
					sprintf( DstString, LT_DEATHMETHOD_NME_POWERLASER );
					break;
				case NO_PRIMARY:
					sprintf( DstString, LT_DEATHMETHOD_NO_PRIMARY );
					break;
				default:
	   				sprintf( DstString, LT_DEATHMETHOD_KILLED );
					break;
			}
			break;
	
		case WEPTYPE_Secondary:
			switch( Weapon )
			{
				case MUGMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_MUG );
					break;
				case SOLARISMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_SOLARIS );
					break;
				case THIEFMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_THEIF );
					break;
				case SCATTERMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_SCATTER );
					break;
				case GRAVGONMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_GRAVGON );
					break;
				case MULTIPLEMISSILE:
	   				sprintf( DstString, LT_DEATHMETHOD_MFRL );
					break;
				case TITANSTARMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_TITAN );
					break;
				case PURGEMINE:
	   				sprintf( DstString, LT_DEATHMETHOD_PURGE_MINE );
					break;
				case PINEMINE:
	   				sprintf( DstString, LT_DEATHMETHOD_PINE_MINE);
					break;
				case QUANTUMMINE:
	   				sprintf( DstString, LT_DEATHMETHOD_QUANTUM_MINE );
					break;
				case SPIDERMINE:
	   				sprintf( DstString, LT_DEATHMETHOD_SPIDER_MINE );
					break;
				case PINEMISSILE:
	   				sprintf( DstString, LT_DEATHMETHOD_PINE_MISSILE );
					break;
				case TITANSTARSHRAPNEL:
					sprintf( DstString, LT_DEATHMETHOD_TITAN_SHRAPNEL );
					break;
				case ENEMYSPIRALMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_ENEMYSPIRALMISSILE );
					break;
				case ENEMYHOMINGMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_NMEHOMINGMISSILE );
					break;
				case ENEMYBLUEHOMINGMISSILE:
					sprintf( DstString, LT_DEATHMETHOD_ENEMYBLUEHOMINGMISSILE );
					break;
				case ENEMYFIREBALL:
					sprintf( DstString, LT_DEATHMETHOD_ENEMYFIREBALL );
					break;
				case ENEMYTENTACLE:
					sprintf( DstString, LT_DEATHMETHOD_ENEMYTENTACLE );
					break;
				case ENEMYDEPTHCHARGE:
					sprintf( DstString, LT_DEATHMETHOD_ENEMYDEPTHCHARGE );
					break;
				case NO_SECONDARY:
					sprintf( DstString, LT_DEATHMETHOD_NO_SECONDARY );
					break;
				default:
	   				sprintf( DstString, LT_DEATHMETHOD_KILLED );
					break;
			}
			break;

		case WEPTYPE_Various:
		default:
			sprintf( DstString, LT_DEATHMETHOD_KILLED );
			break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop Shield pickup
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DropShield( void )
{
	float	Speed;
	float	LifeCount;
	int16	PickupNum;
	VECTOR	DirVector;
	uint16	i;

	if( Ships[ WhoIAm ].Object.Shield >= 32 )
	{
		Ships[ WhoIAm ].Object.Shield -= 32;

		ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &DirVector );
		PickupNum = PICKUP_Shield;
		LifeCount = -1.0F;
		Speed = PICKUP_SPEED;

		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );

		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Drop Ammo pickup
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DropAmmo( void )
{
	float	Speed;
	float	LifeCount;
	int16	PickupNum;
	VECTOR	DirVector;
	BOOL	DropFlag = FALSE;
	uint16	i;

	if( ( Ships[ WhoIAm ].Object.Flags & SHIP_SuperNashram ) )
	{
		switch( Ships[ WhoIAm ].Primary )
		{
			case PULSAR:
			case TROJAX:
			case TRANSPULSE_CANNON:
			case LASER:
				if( CopyOfGeneralAmmo > GENERALAMMOPERPOD )
				{
					DropFlag = TRUE;
					CopyOfGeneralAmmo -= GENERALAMMOPERPOD;
					PickupNum = PICKUP_GeneralAmmo;
				}
				break;
			case PYROLITE_RIFLE:
				if( CopyOfPyroliteAmmo > PYROLITEAMMOPERPOD )
				{
					DropFlag = TRUE;
					CopyOfPyroliteAmmo -= PYROLITEAMMOPERPOD;
					PickupNum = PICKUP_PyroliteAmmo;
				}
				break;
			case SUSS_GUN:
				if( CopyOfSussGunAmmo > SUSSGUNAMMOPERPOD )
				{
					DropFlag = TRUE;
					CopyOfSussGunAmmo -= SUSSGUNAMMOPERPOD;
					PickupNum = PICKUP_SussGunAmmo;
				}
				break;
		}
	}
	else
	{
		switch( Ships[ WhoIAm ].Primary )
		{
			case PULSAR:
			case TROJAX:
			case TRANSPULSE_CANNON:
			case LASER:
				if( GeneralAmmo > GENERALAMMOPERPOD )
				{
					DropFlag = TRUE;
					GeneralAmmo -= GENERALAMMOPERPOD;
					PickupNum = PICKUP_GeneralAmmo;
				}
				break;
			case PYROLITE_RIFLE:
				if( PyroliteAmmo > PYROLITEAMMOPERPOD )
				{
					DropFlag = TRUE;
					PyroliteAmmo -= PYROLITEAMMOPERPOD;
					PickupNum = PICKUP_PyroliteAmmo;
				}
				break;
			case SUSS_GUN:
				if( SussGunAmmo > SUSSGUNAMMOPERPOD )
				{
					DropFlag = TRUE;
					SussGunAmmo -= SUSSGUNAMMOPERPOD;
					PickupNum = PICKUP_SussGunAmmo;
				}
				break;
		}
	}

	if( DropFlag )
	{
		ApplyMatrix( &Ships[WhoIAm].Object.FinalMat, &Forward, &DirVector );
		LifeCount = -1.0F;
		Speed = PICKUP_SPEED;

		i = InitOnePickup( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group, &DirVector, Speed, PickupNum, WhoIAm, ++Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );

		if( ( i != (uint16) -1 ) && ( i != (uint16) -2 ) )
		{
			DropPickupSend( &Ships[ WhoIAm ].Object.Pos, Ships[ WhoIAm ].Object.Group ,&DirVector, Speed, PickupNum, Ships[ WhoIAm ].PickupIdCount, -1, FALSE, LifeCount, (uint16) -1 );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of Transpulse
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	NewPos
				:	VECTOR	*	MoveOffset
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
BOOL ChangeTranspulseDir( uint16 i, VECTOR * Pos, VECTOR * MoveOffset, VECTOR * Dir )
{
	uint16	Ship;
	uint16	Enemy;
	uint16	Secondary;
	uint16	fmpoly;
	QUAT	TempQuat;
	MATRIX	TempMatrix;
	VECTOR	NewDir;
	float	ClosestCos = -1.0F;
	uint16	NewGroup;

	NewGroup = MoveGroup( &Mloadheader, &PrimBulls[i].Pos, PrimBulls[i].GroupImIn, MoveOffset );

	if( !PrimBulls[i].Bounces ) return( FALSE );
	PrimBulls[i].Bounces--;

	PrimBulls[i].Speed *= 0.5F;

#ifdef SINT_PEACEFROG_CHEAT
	TargetingWeaponCheat = -1;
#endif
	switch( PrimBulls[i].OwnerType )
	{
		case OWNER_SHIP:
			Ship = TargetClosestShip( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			Enemy = TargetClosestEnemy( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			if( Enemy != (uint16) -1 ) Ship = (uint16) -1;
			Secondary = TargetClosestSecondary( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			if( Secondary != (uint16) -1 )
			{
				Ship = (uint16) -1;
				Enemy = (uint16) -1;
			}
			break;

		case OWNER_ENEMY:
			Enemy = (uint16) -1;
			Ship = TargetClosestShip( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			Secondary = TargetClosestSecondary( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			if( Secondary != (uint16) -1 )
			{
				Ship = (uint16) -1;
			}
			break;

		case OWNER_NOBODY:
			Enemy = (uint16) -1;
			Ship = TargetClosestShip( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			Secondary = TargetClosestSecondary( Pos, Dir, NewGroup, PrimBulls[i].OwnerType, PrimBulls[i].Owner, (float) COSD( TRANSPULSE_VIEWCONE ), &ClosestCos );
			if( Secondary != (uint16) -1 )
			{
				Ship = (uint16) -1;
			}
			break;

		case OWNER_MINE:
		default:
			Ship = (uint16) -1;
			Enemy = (uint16) -1;
			Secondary = (uint16) -1;
			break;
	}

	if( Ship != (uint16) -1 )
	{
		NewDir.x = ( Ships[ Ship ].Object.Pos.x - Pos->x );
		NewDir.y = ( Ships[ Ship ].Object.Pos.y - Pos->y );
		NewDir.z = ( Ships[ Ship ].Object.Pos.z - Pos->z );
		NormaliseVector( &NewDir );
	}
	else
	{
		if( Enemy != (uint16) -1 )
		{
			NewDir.x = ( Enemies[ Enemy ].Object.Pos.x - Pos->x );
			NewDir.y = ( Enemies[ Enemy ].Object.Pos.y - Pos->y );
			NewDir.z = ( Enemies[ Enemy ].Object.Pos.z - Pos->z );
			NormaliseVector( &NewDir );

			Enemies[ Enemy ].AIFlags |= AI_TARGETED_ONEOFF;
			Enemies[ Enemy ].TargetingPos = *Pos;
			Enemies[ Enemy ].TargetingDir = NewDir;
		}
		else
		{
			if( Secondary != (uint16) -1 )
			{
				NewDir.x = ( SecBulls[ Secondary ].Pos.x - Pos->x );
				NewDir.y = ( SecBulls[ Secondary ].Pos.y - Pos->y );
				NewDir.z = ( SecBulls[ Secondary ].Pos.z - Pos->z );
				NormaliseVector( &NewDir );
			}
			else
			{
				NewDir = *Dir;
			}
		}
	}

	PrimBulls[i].ColFlag = 0;
	PrimBulls[i].Dir = NewDir;

	fmpoly = PrimBulls[i].fmpoly;					// Faceme polygon attached

	if( fmpoly != (uint16 ) -1 )
	{
		QuatFrom2Vectors( &TempQuat, &FmPolys[ fmpoly ].UpVector, &NewDir );
		QuatToMatrix( &TempQuat, &TempMatrix );
		ApplyMatrix( &TempMatrix, &FmPolys[ fmpoly ].DirVector, &FmPolys[ fmpoly ].DirVector );
		FmPolys[ fmpoly ].UpVector = NewDir;
		FmPolys[ fmpoly ].xsize *= 0.5F;
	}
	return( TRUE );
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of Pulsar
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangePulsarDir( uint16 i, VECTOR * Dir )
{
	float	Trail_Length;
	uint16	poly;
	QUAT	TempQuat;
	MATRIX	TempMatrix;
	VECTOR	DirVector;
	VECTOR	DownVector;
	VECTOR	LeftVector;

	QuatFromVector( Dir, &TempQuat );
	QuatToMatrix( &TempQuat, &TempMatrix );

	ApplyMatrix( &TempMatrix, &Forward, &DirVector );
	ApplyMatrix( &TempMatrix, &SlideDown, &DownVector );
	ApplyMatrix( &TempMatrix, &SlideLeft, &LeftVector );
	
	PrimBulls[i].ColFlag = 0;
	PrimBulls[i].Dir = *Dir;

	Trail_Length = ( (float) -( ( PrimBulls[ i ].PowerLevel * PULSAR_TAILPOWERMUL ) + PULSAR_TAILADDMIN ) );
	
	poly = PrimBulls[i].poly;
	
	if( poly != (uint16) -1 )
	{
		Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( DownVector.x * PULSAR_TAILSTART );
		Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( DownVector.y * PULSAR_TAILSTART );
		Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( DownVector.z * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( DownVector.x * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( DownVector.y * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( DownVector.z * PULSAR_TAILSTART );
		Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Trail_Length ) + ( DownVector.x * PULSAR_TAILEND );
		Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Trail_Length ) + ( DownVector.y * PULSAR_TAILEND );
		Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Trail_Length ) + ( DownVector.z * PULSAR_TAILEND );
		Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Trail_Length ) - ( DownVector.x * PULSAR_TAILEND );
		Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Trail_Length ) - ( DownVector.y * PULSAR_TAILEND );
		Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Trail_Length ) - ( DownVector.z * PULSAR_TAILEND );
		poly = Polys[ poly ].Prev;
	}

	if( poly != (uint16) -1 )
	{
		Polys[ poly ].Pos1.x = PrimBulls[i].Pos.x - ( LeftVector.x * PULSAR_TAILSTART );
		Polys[ poly ].Pos1.y = PrimBulls[i].Pos.y - ( LeftVector.y * PULSAR_TAILSTART );
		Polys[ poly ].Pos1.z = PrimBulls[i].Pos.z - ( LeftVector.z * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.x = PrimBulls[i].Pos.x + ( LeftVector.x * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.y = PrimBulls[i].Pos.y + ( LeftVector.y * PULSAR_TAILSTART );
		Polys[ poly ].Pos2.z = PrimBulls[i].Pos.z + ( LeftVector.z * PULSAR_TAILSTART );
		Polys[ poly ].Pos3.x = PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Trail_Length ) + ( LeftVector.x * PULSAR_TAILEND );
		Polys[ poly ].Pos3.y = PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Trail_Length ) + ( LeftVector.y * PULSAR_TAILEND );
		Polys[ poly ].Pos3.z = PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Trail_Length ) + ( LeftVector.z * PULSAR_TAILEND );
		Polys[ poly ].Pos4.x = PrimBulls[i].Pos.x + ( PrimBulls[ i ].Dir.x * Trail_Length ) - ( LeftVector.x * PULSAR_TAILEND );
		Polys[ poly ].Pos4.y = PrimBulls[i].Pos.y + ( PrimBulls[ i ].Dir.y * Trail_Length ) - ( LeftVector.y * PULSAR_TAILEND );
		Polys[ poly ].Pos4.z = PrimBulls[i].Pos.z + ( PrimBulls[ i ].Dir.z * Trail_Length ) - ( LeftVector.z * PULSAR_TAILEND );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of Pulsar
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangeTrojaxDir( uint16 i, VECTOR * Dir )
{
	uint16	fmpoly;
	QUAT	TempQuat;
	MATRIX	TempMatrix;

	PrimBulls[i].ColFlag = 0;
	PrimBulls[i].Dir = *Dir;

	fmpoly = PrimBulls[i].fmpoly;

	if( fmpoly != (uint16 ) -1 )
	{
		QuatFrom2Vectors( &TempQuat, &Forward, Dir );
		QuatToMatrix( &TempQuat, &TempMatrix );
		FmPolys[ fmpoly ].DirVector = *Dir;
		FmPolys[ fmpoly ].Mat = TempMatrix;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of Pulsar
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangeSussgunDir( uint16 i, VECTOR * Dir )
{
	PrimBulls[i].ColFlag = 0;
	PrimBulls[i].Dir = *Dir;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of OrbitPulsar
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ChangeOrbitPulsarDir( uint16 i, VECTOR * Dir )
{
	PrimBulls[i].ColFlag = 0;
	PrimBulls[i].Dir = *Dir;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Change Direction of Laser
	Input		:	uint16		Primary Bullet Index
				:	VECTOR	*	New Direction
	Output		:	BOOL		True/False
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReflectLaser( uint16 i, VECTOR * Dir )
{
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Give me Super Nashram weapon
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GivemeSuperNashram( void )
{
	int16	Count;

	if( ( WhoIAm < 0 ) || ( WhoIAm >= ( MAX_PLAYERS + 1 ) ) ) return;

	CopyOfPowerLevel = Ships[ WhoIAm ].Object.PowerLevel;
	CopyOfGeneralAmmo = GeneralAmmo;
	CopyOfSussGunAmmo = SussGunAmmo;
	CopyOfPyroliteAmmo = PyroliteAmmo;
	CopyOfNitroFuel = NitroFuel;
	CopyOfOrbitals = Ships[ WhoIAm ].NumMultiples;
	for( Count = 0; Count <  MAXMULTIPLES; Count++ ) CopyOfOrbAmmo[ Count ] = Ships[ WhoIAm ].OrbAmmo[ Count ];

	Ships[ WhoIAm ].Object.PowerLevel = ( MAXPOWERLEVELS - 1 );

	for( Count = 0; Count < 4; Count++ )
	{
		Ships[ WhoIAm ].OrbAmmo[ Count ] = MAXMULTIPLEAMMO;
	}

	Ships[ WhoIAm ].NumMultiples = 4;
	Ships[ WhoIAm ].Object.Flags |= SHIP_SuperNashram;
	Ships[ WhoIAm ].SuperNashramTimer = ( 30.0F * 60.0F );

	CreateOrbitPulsar( WhoIAm );

	PyroliteAmmo = MAXPYROLITEAMMO;		//MAXPYROLITEAMMO;
	SussGunAmmo = MAXSUSSGUNAMMO;		//MAXSUSSGUNAMMO;
	GeneralAmmo = MAXGENERALAMMO;
	NitroFuel = 100.0F;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Lose Super Nashram weapon
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void LoseSuperNashram( void )
{
	int16	Count;

	if( ( WhoIAm < 0 ) || ( WhoIAm >= ( MAX_PLAYERS + 1 ) ) ) return;

	for( Count = 0; Count < MAXMULTIPLES; Count++ ) Ships[ WhoIAm ].OrbAmmo[ Count ] = CopyOfOrbAmmo[ Count ];

	Ships[ WhoIAm ].NumMultiples = CopyOfOrbitals;
	CreateOrbitPulsar( WhoIAm );

	Ships[ WhoIAm ].Object.PowerLevel = CopyOfPowerLevel;
	GeneralAmmo = CopyOfGeneralAmmo;
	SussGunAmmo = CopyOfSussGunAmmo;
	PyroliteAmmo = CopyOfPyroliteAmmo;
	NitroFuel = CopyOfNitroFuel;

	Ships[ WhoIAm ].Object.Flags &= ~SHIP_SuperNashram;
	Ships[ WhoIAm ].SuperNashramTimer = 0.0F;

	if( !GodMode )
	{
		NumSuperNashrams++;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Give me Super Nashram weapon
	Input		:	uint16 Player
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HostGivemeSuperNashram( uint16 Player )
{
	int16	Count;

	if( ( Player < 0 ) || ( Player >= ( MAX_PLAYERS + 1 ) ) ) return;

	Host_CopyOfPowerLevel[ Player ] = Host_PowerLevel[ Player ];
	Host_CopyOfGeneralAmmo[ Player ] = Host_GeneralAmmo[ Player ];
	Host_CopyOfSussGunAmmo[ Player ] = Host_SussGunAmmo[ Player ];
	Host_CopyOfPyroliteAmmo[ Player ] = Host_PyroliteAmmo[ Player ];
	Host_CopyOfNitroFuel[ Player ] = Host_NitroFuel[ Player ];
	Host_CopyOfNumOfOrbitals[ Player ] = Host_NumOfOrbitals[ Player ];

	Host_PowerLevel[ Player ] = ( MAXPOWERLEVELS - 1 );

	for( Count = 0; Count <  MAXMULTIPLES; Count++ )
	{
		Host_CopyOfOrbAmmo[ Player ][ Count ] = Host_OrbAmmo[ Player ][ Count ];
	}

//	Ships[ Player ].Object.PowerLevel = ( MAXPOWERLEVELS - 1 );

	for( Count = 0; Count < 4; Count++ )
	{
		Host_OrbAmmo[ Player ][ Count ] = MAXMULTIPLEAMMO;
	}

//	Ships[ Player ].NumMultiples = 4;
//	Ships[ Player ].Object.Flags |= SHIP_SuperNashram;
	Ships[ Player ].SuperNashramTimer = ( 30.0F * 60.0F );

//	CreateOrbitPulsar( WhoIAm );

	Host_PyroliteAmmo[ Player ] = MAXPYROLITEAMMO;		//MAXPYROLITEAMMO;
	Host_SussGunAmmo[ Player ] = MAXSUSSGUNAMMO;		//MAXSUSSGUNAMMO;
	Host_GeneralAmmo[ Player ] = MAXGENERALAMMO;
	Host_NitroFuel[ Player ] = 100.0F;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Lose Super Nashram weapon
	Input		:	uint16	Player
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void HostLoseSuperNashram( uint16 Player )
{
	int16	Count;

	if( ( Player < 0 ) || ( Player >= ( MAX_PLAYERS + 1 ) ) ) return;

	for( Count = 0; Count < MAXMULTIPLES; Count++ )
	{
		Host_OrbAmmo[ Player ][ Count ] = Host_CopyOfOrbAmmo[ Player ][ Count ];
	}

	Host_NumOfOrbitals[ Player ] = Host_CopyOfNumOfOrbitals[ Player ];
	Host_PowerLevel[ Player ] = Host_CopyOfPowerLevel[ Player ];
	Host_GeneralAmmo[ Player ] = Host_CopyOfGeneralAmmo[ Player ];
	Host_SussGunAmmo[ Player ] = Host_CopyOfSussGunAmmo[ Player ];
	Host_PyroliteAmmo[ Player ] = Host_CopyOfPyroliteAmmo[ Player ];
	Host_NitroFuel[ Player ] = Host_CopyOfNitroFuel[ Player ];

	Ships[ Player ].SuperNashramTimer = 0.0F;

	NumSuperNashrams++;
}


/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Shield Effect when Invulnerable
	Input		:	VECTOR	*	Center of Shield
				:	VECTOR	*	Intersection Point ( Front of shield )
				:	VECTOR	*	Intersection Point2 ( Back of shield )
				:	uint16		ShipHit
				:	int16		Number of Spirals
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float	InvulInterval = 1.0F;
float	InvulCount = 0.0F;

void CreateInvEffect( uint16 ShipHit, int16 Num, uint8 RVal, uint8 GVal, uint8 BVal )
{
	uint16  Poly;
	float	Step;
	float	Angle;
	float	Time;
	QUAT	StartQuat;
	QUAT	EndQuat;
	VECTOR	TempVector;
	VECTOR	TempVector2;
	MATRIX	TempMat;
	MATRIX	TempInvMat;

	InvulInterval = 1.0F;

	if( AutoDetail )
	{
		if( avgframelag > 2.0F ) InvulInterval +=  ( avgframelag - 1.0F );
	}

	InvulCount += framelag;
		
	if( InvulCount >= InvulInterval )
	{
		InvulCount = 0.0F;

		if( IsGroupVisible[ Ships[ ShipHit ].Object.Group ] )
		{
			while( Num-- )
			{
				TempVector.x = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				TempVector.y = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				TempVector.z = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				NormaliseVector( &TempVector );
			
				TempVector2.x = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				TempVector2.y = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				TempVector2.z = ( ( ( (float) Random_Range( 5120 ) ) / 20480.0F ) - 0.125F );
				NormaliseVector( &TempVector2 );
			
				Angle = (float) acos( DotProduct( &TempVector, &TempVector2 ) );
				if( Angle ) Time = D2R( ( 2.0F / Angle ) );
				else Time = 1.0F;
				if( Time > 1.0F ) Time = 1.0F;
			
				QuatFrom2Vectors( &StartQuat, &Forward, &TempVector );
				QuatFrom2Vectors( &EndQuat, &Forward, &TempVector2 );
			
				Step = 0.0F;
			
				Poly = FindFreePoly();
					
				if( Poly != (uint16) -1 )
				{
					Polys[ Poly ].Col1.R = RVal;
					Polys[ Poly ].Col1.G = GVal;
					Polys[ Poly ].Col1.B = BVal;
					Polys[ Poly ].Col2.R = RVal;
					Polys[ Poly ].Col2.G = GVal;
					Polys[ Poly ].Col2.B = BVal;
					Polys[ Poly ].Col3.R = RVal;
					Polys[ Poly ].Col3.G = GVal;
					Polys[ Poly ].Col3.B = BVal;
					Polys[ Poly ].Col4.R = RVal;
					Polys[ Poly ].Col4.G = GVal;
					Polys[ Poly ].Col4.B = BVal;
					Polys[ Poly ].Trans1 = 255;
					Polys[ Poly ].Trans2 = 255;
					Polys[ Poly ].Trans3 = 255;
					Polys[ Poly ].Trans4 = 255;
					Polys[ Poly ].Scale = 0.5F;
					Polys[ Poly ].Flags = POLY_FLAG_TWOSIDED;
					Polys[ Poly ].Frm_Info = &Bits_Header;
					Polys[ Poly ].Frame = 0.0F;
					Polys[ Poly ].Quat = StartQuat;
					Polys[ Poly ].Qlerp.start = StartQuat;			/* Starting Quaternion */
					Polys[ Poly ].Qlerp.end = EndQuat;				/* Ending Quaternion */
					Polys[ Poly ].Qlerp.crnt = &Polys[ Poly ].Quat;	/* Address to store new Quaternion */
					Polys[ Poly ].Qlerp.dir = QuatDotProduct( &Polys[ Poly ].Qlerp.start, &Polys[ Poly ].Qlerp.end );
					Polys[ Poly ].Qlerp.time = Step;
					Polys[ Poly ].SeqNum = POLY_INVBITS;
					Polys[ Poly ].Ship = ShipHit;
					Polys[ Poly ].TimeStep = Time;
					Polys[ Poly ].Group = Ships[ ShipHit ].Object.Group;
				
					QuatInterpolate( &Polys[ Poly ].Qlerp );
			
					QuatToMatrix( &Polys[ Poly ].Quat, &TempMat );
					MatrixTranspose( &TempMat, &TempInvMat );
					TempInvMat._41 = Ships[ Polys[ Poly ].Ship ].Object.Pos.x;
					TempInvMat._42 = Ships[ Polys[ Poly ].Ship ].Object.Pos.y;
					TempInvMat._43 = Ships[ Polys[ Poly ].Ship ].Object.Pos.z;
			
					VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 0 ], &Polys[ Poly ].Pos1 );
					VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 1 ], &Polys[ Poly ].Pos2 );
					VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 2 ], &Polys[ Poly ].Pos3 );
					VisPolyApplyMatrix( &TempInvMat, &ShieldVerts[ 3 ], &Polys[ Poly ].Pos4 );

					AddPolyToTPage( Poly, GetTPage( *Polys[ Poly ].Frm_Info, (int16) Polys[ Poly ].Frame ) );
				}
			}
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fire Primary Bullet used for enemies/others
	Input		:	uint16		OwnerType
				:	uint16		OwnerID
				:	uint16		BulletID
				:	uint16		Weapon
				:	uint16		Group
				:	VECTOR	*	Pos
				:	VECTOR	*	Offset
				:	VECTOR	*	DirVector
				:	VECTOR	*	UpVector
				:	int16		PowerLevel
				:	float		PowerLevel ( Charging )
				:	BOOL		NetFlag ( Send to everyone )
				:	GUNBOJECT *	Gun ( Enemies Only )
	Output		:	nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint16 EnemyFirePrimary( uint16 OwnerType, uint16 OwnerID, uint16 BulletID, int8 Weapon,
						uint16 Group, VECTOR * Pos, VECTOR * Offset, VECTOR * Dir, VECTOR * Up,
						int16 PowerLevel, float PLevel, BOOL NetFlag, void * Gun )
{
	int16	Count;
	uint16	i = (uint16) -1;

	switch( Weapon )
	{
		case SUSS_GUN:
		case NME_SUSS_GUN:
			for( Count = 0; Count < 4; Count++ )
			{
				i = InitOnePrimBull( OwnerType, OwnerID, BulletID, Weapon, Group, Pos, Offset,
											 Dir, Up, PowerLevel, PLevel, NetFlag );
			}
			break;

		case PULSAR:
		case TROJAX:
		case PYROLITE_RIFLE:
		case TRANSPULSE_CANNON:
		case LASER:
		case ORBITPULSAR:
		case NME_BULLET1:
		case NME_PULSAR:
		case NME_TROJAX:
		case NME_PYROLITE:
		case NME_TRANSPULSE:
		case NME_LASER:
		case NME_POWERLASER:
		case NME_LIGHTNING:
		case FLAMES:
			i = InitOnePrimBull( OwnerType, OwnerID, BulletID, Weapon, Group, Pos, Offset,
									 Dir, Up, PowerLevel, PLevel, NetFlag );
			if( i != (uint16) -1 )
			{
				PrimBulls[i].EnemyGun = (GUNOBJECT *) Gun;
				GetLaserLocalVector( i, &PrimBulls[i].LocalDir );
			}
			break;
	}

	return( i );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create PowerLaser Sparks
	Input		:	VECTOR	*	Position
				:	VECTOR	*	Direction
				:	uint16		Group
				:	uint8		Red
				:	uint8		Green
				:	uint8		Blue
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreatePowerLaserSparks( VECTOR * Pos, VECTOR * Dir, uint16 Group, uint8 Red, uint8 Green, uint8 Blue )
{
	uint16	fmpoly;
	uint16	light;
	int16	NumSparks;
	int16	Count;

	light = FindFreeXLight();

	if( light != (uint16 ) -1 )
	{
		XLights[ light ].Visible = TRUE;
		XLights[ light ].Pos = *Pos;
		XLights[ light ].Size = ( 1536.0F * GLOBAL_SCALE );
		XLights[ light ].SizeCount = ( 153.6F * GLOBAL_SCALE );
		XLights[ light ].ColorCount = 25.0F;
		XLights[ light ].r = (float) Red;
		XLights[ light ].g = (float) Green;
		XLights[ light ].b = (float) Blue;
		XLights[ light ].Group = Group;
	}

	fmpoly = FindFreeFmPoly();					// Faceme polygon attached
	
	if( fmpoly != (uint16 ) -1 )
	{
		FmPolys[ fmpoly ].LifeCount = 4.0F;
		FmPolys[ fmpoly ].Pos = *Pos;
		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKSFACEME;
		FmPolys[ fmpoly ].Frame = 0.0F;
		FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
		FmPolys[ fmpoly ].xsize = ( 16.0F * GLOBAL_SCALE );
		FmPolys[ fmpoly ].ysize = ( 16.0F * GLOBAL_SCALE );
#if ACTUAL_TRANS
	  	FmPolys[ fmpoly ].Trans = 128;
#else
	  	FmPolys[ fmpoly ].Trans = 255;
#endif
	   	FmPolys[ fmpoly ].R = Red;
	   	FmPolys[ fmpoly ].G = Green;
	   	FmPolys[ fmpoly ].B = Blue;
		FmPolys[ fmpoly ].Start_R = Red;
		FmPolys[ fmpoly ].Start_G = Green;
		FmPolys[ fmpoly ].Start_B = Blue;
		FmPolys[ fmpoly ].Frm_Info = &Flare_Header;
		FmPolys[ fmpoly ].Group = Group;
		AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	}
	
	NumSparks = ( Random_Range( 10 ) + 10 );
	
	for( Count = 0; Count < NumSparks; Count++ )
	{
	   	fmpoly = FindFreeFmPoly();
	
	   	if( fmpoly != (uint16 ) -1 )
	   	{
			FmPolys[ fmpoly ].LifeCount = 1000.0F;
	   		FmPolys[ fmpoly ].Pos = *Pos;
	   		FmPolys[ fmpoly ].Dir = *Dir;
	   		FmPolys[ fmpoly ].SeqNum = FM_SPOTFX_SPARKS;
			FmPolys[ fmpoly ].AnimSpeed = 10.0F;
	   		FmPolys[ fmpoly ].Frame = 0.0F;
	   		FmPolys[ fmpoly ].Frm_Info = &Bits_Header;
			FmPolys[ fmpoly ].Flags = FM_FLAG_ZERO;
			FmPolys[ fmpoly ].Trans = 255;
	   		FmPolys[ fmpoly ].Dir.x += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   		FmPolys[ fmpoly ].Dir.y += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
	   		FmPolys[ fmpoly ].Dir.z += ( ( ( (float) Random_Range( 5120 ) ) / 5120.0F ) - 0.5F );
			FmPolys[ fmpoly ].R = Red;
			FmPolys[ fmpoly ].G = Green;
			FmPolys[ fmpoly ].B = Blue;
			FmPolys[ fmpoly ].Start_R = Red;
			FmPolys[ fmpoly ].Start_G = Green;
			FmPolys[ fmpoly ].Start_B = Blue;
			FmPolys[ fmpoly ].Speed = ( ( ( (float) Random_Range( 5120 ) ) / 640.F ) + 10.0F ) * GLOBAL_SCALE;
			FmPolys[ fmpoly ].Rot = 0.0F;
			FmPolys[ fmpoly ].xsize = ( 10.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].ysize = ( 10.0F * GLOBAL_SCALE );
			FmPolys[ fmpoly ].Group = Group;
			NormaliseVector( &FmPolys[ fmpoly ].Dir );
			AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
	   	}
		else
		{
			return;
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Local Vector for laser
	Input		:	uint16		Primary bullet index
				:	VECTOR	*	Local Vector (TBFI)
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void GetLaserLocalVector( uint16 i, VECTOR * LocalVector )
{
	PVSPOTFX	*	SpotFXPtr;
	GUNOBJECT	*	GunPtr;		// the first gun...

	switch( PrimBulls[i].Weapon )
	{
		case LASER:
		case NME_LASER:
		case NME_LIGHTNING:
		case NME_POWERLASER:
			switch( PrimBulls[i].OwnerType )
			{
				case OWNER_SHIP:
					ApplyMatrix( &Ships[ PrimBulls[i].Owner ].Object.FinalInvMat, &PrimBulls[i].Dir, LocalVector );
					break;

				case OWNER_ENEMY:
					if( !Enemies[ PrimBulls[i].Owner ].Object.FirstGun )
					{
		   				ApplyMatrix( &Enemies[ PrimBulls[i].Owner ].Object.FinalInvMat, &PrimBulls[i].Dir, LocalVector );
					}
					else
					{
						GunPtr = PrimBulls[i].EnemyGun;
						if( GunPtr )
						{
							ApplyMatrix( &GunPtr->InvMat, &PrimBulls[i].Dir, LocalVector );
						}
					}
					break;

				case OWNER_MODELSPOTFX:
					if( ModelNames[ Models[ PrimBulls[i].Owner ].ModelNum ].DoIMorph )
					{
						SpotFXPtr = ( MxaModelHeaders[ Models[ PrimBulls[i].Owner ].ModelNum ].SpotFX + PrimBulls[i].SpotFX );
					}
					else
					{
						SpotFXPtr = ( ModelHeaders[ Models[ PrimBulls[i].Owner ].ModelNum ].SpotFX + PrimBulls[i].SpotFX );
					}

					ApplyMatrix( &Models[ PrimBulls[i].Owner ].InvMat, &PrimBulls[i].Dir, LocalVector );
					break;

				case OWNER_NOBODY:
				default:
					*LocalVector = PrimBulls[i].Dir;
					break;

			}
	 		break;

		default:
			break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Create Explosion
	Input		:	VECTOR	*	Position
				:	uint16		Group
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CreateScaleExplosion( VECTOR * Pos, uint16 Group, float GlobalScale )
{
	uint16	fmpoly;
	int16	Count;
	float	Scale;

	if( IsGroupVisible[ Group ] )
	{
		for( Count = 0; Count < ( Random_Range( 2 ) + 1 ); Count++ )
		{
			fmpoly = FindFreeFmPoly();
	
			if( fmpoly != (uint16 ) -1 )
			{
				FmPolys[ fmpoly ].LifeCount = 1000.0F;
				FmPolys[ fmpoly ].Pos = *Pos;
				FmPolys[ fmpoly ].SeqNum = FM_ANIM;
				FmPolys[ fmpoly ].Frame = 0.0F;
				FmPolys[ fmpoly ].Frm_Info = &Exp_Header;
				FmPolys[ fmpoly ].Flags = FM_FLAG_MOVEOUT;
				FmPolys[ fmpoly ].R = 192;
				FmPolys[ fmpoly ].G = 192;
				FmPolys[ fmpoly ].B = 192;
#if ACTUAL_TRANS
				FmPolys[ fmpoly ].Trans = 128;
#else
				FmPolys[ fmpoly ].Trans = 255;
#endif
				FmPolys[ fmpoly ].Dir.x = 0.0F;
				FmPolys[ fmpoly ].Dir.y = 0.0F;
				FmPolys[ fmpoly ].Dir.z = 0.0F;
				FmPolys[ fmpoly ].Speed = 0.0F;
				FmPolys[ fmpoly ].Group = Group;
	
				if( Count == 0 )
				{
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					FmPolys[ fmpoly ].xsize = ( NORM_EXPLO_SIZE * GlobalScale );
					FmPolys[ fmpoly ].ysize = ( NORM_EXPLO_SIZE * GlobalScale );
				}
				else
				{
					FmPolys[ fmpoly ].Pos.x += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.y += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Pos.z += ( ( ( (float) Random_Range( 256 ) ) - 128.0F ) * GLOBAL_SCALE );
					FmPolys[ fmpoly ].Rot = (float) Random_Range( 360 );
					Scale = (float) ( Random_Range( 10 ) + 6 ) * GLOBAL_SCALE;
					Scale *= GlobalScale;
					FmPolys[ fmpoly ].xsize = Scale;
					FmPolys[ fmpoly ].ysize = Scale;
				}
				AddFmPolyToTPage( fmpoly, GetTPage( *FmPolys[ fmpoly ].Frm_Info, 0 ) );
			}
			else
			{
				return;
			}
		}
	}

	PlayPannedSfx( SFX_BangBang, Group , Pos, 0.0F );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Check if Ammo Pickup needed, if so which one
	Input		:	Nothing
	Output		:	int16	Ammo Pickup to Generate ( -1 None )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16 GetAmmoPickupToGen( void )
{
	float	AmmoLevels[ MAXPRIMARYWEAPONS ];
	float	TotalAmmo = 0.0F;
	int16	Count;

	AmmoLevels[ PULSAR ] = GeneralAmmo;
	AmmoLevels[ TROJAX ] = 0.0F;					//GeneralAmmo;
	AmmoLevels[ PYROLITE_RIFLE ] = PyroliteAmmo;
	AmmoLevels[ TRANSPULSE_CANNON ] = 0.0F;			//GeneralAmmo;
	AmmoLevels[ SUSS_GUN ] = SussGunAmmo;
	AmmoLevels[ LASER ] = 0.0F;						//GeneralAmmo;

	for( Count = 0; Count < MAXPRIMARYWEAPONS; Count++ )
	{
		if( PrimaryWeaponsGot[ Count ] )
		{
			TotalAmmo += AmmoLevels[ Count ];
		}
	}

	if( TotalAmmo < MINIMUM_TOTAL_AMMO )
	{
		return( PICKUP_GeneralAmmo );
	}

	return( -1 );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Save PrimBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * SavePrimBulls( FILE * fp )
{
	uint16	i;
	uint16	TempIndex;

	if( fp )
	{
		for( i = 0; i < MAXPRIMARYWEAPONS; i++ )
		{
			fwrite( &PrimaryWeaponsGot[ i ], sizeof( int16 ), 1, fp );
		}

		fwrite( &FirstPrimBullUsed, sizeof( FirstPrimBullUsed ), 1, fp );
		fwrite( &FirstPrimBullFree, sizeof( FirstPrimBullFree ), 1, fp );
		fwrite( &PrimaryFireDelay, sizeof( PrimaryFireDelay ), 1, fp );
		fwrite( &OrbitFireDelay, sizeof( OrbitFireDelay ), 1, fp );
		fwrite( &GeneralAmmo, sizeof( GeneralAmmo ), 1, fp );
		fwrite( &SussGunAmmo, sizeof( SussGunAmmo ), 1, fp );
		fwrite( &PyroliteAmmo, sizeof( PyroliteAmmo ), 1, fp );
		fwrite( &GeneralAmmoUsed, sizeof( GeneralAmmoUsed ), 1, fp );
		fwrite( &SussGunAmmoUsed, sizeof( SussGunAmmoUsed ), 1, fp );
		fwrite( &PyroliteAmmoUsed, sizeof( PyroliteAmmoUsed ), 1, fp );
		fwrite( &LaserTemperature, sizeof( LaserTemperature ), 1, fp );
		fwrite( &LaserOverheated, sizeof( LaserOverheated ), 1, fp );
		fwrite( &PowerLevel, sizeof( PowerLevel ), 1, fp );
		fwrite( &Toggle, sizeof( Toggle ), 1, fp );
		fwrite( &GlobalPrimBullsID, sizeof( GlobalPrimBullsID ), 1, fp );

		i = FirstPrimBullUsed;

		while( i != (uint16) -1 )
		{
			fwrite( &PrimBulls[ i ].Used, sizeof( BOOL ), 1, fp );
			fwrite( &PrimBulls[ i ].Next, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Prev, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Type, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Owner, sizeof( uint16 ), 1, fp );

			if( PrimBulls[ i ].EnemyGun )
			{
				TempIndex = (uint16) ( ( (GUNOBJECT *) PrimBulls[ i ].EnemyGun ) - Enemies[ PrimBulls[ i ].Owner ].Object.FirstGun );
			}
			else
			{
				TempIndex = (uint16) -1;
			}
			fwrite( &TempIndex, sizeof( TempIndex ), 1, fp );

			fwrite( &PrimBulls[ i ].ID, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Weapon, sizeof( int8 ), 1, fp );
			fwrite( &PrimBulls[ i ].PowerLevel, sizeof( int16 ), 1, fp );
			fwrite( &PrimBulls[ i ].TrojPower, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].Speed, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].ColType, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].LocalDir, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fwrite( &PrimBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].ColPoint, sizeof( VERT ), 1, fp );
			fwrite( &PrimBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fwrite( &PrimBulls[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].GroupImIn, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fwrite( &PrimBulls[ i ].line, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].fmpoly, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].numfmpolys, sizeof( int16 ), 1, fp );
			fwrite( &PrimBulls[ i ].poly, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].numpolys, sizeof( int16 ), 1, fp );
			fwrite( &PrimBulls[ i ].light, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].r, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].g, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].b, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].Bounces, sizeof( uint16 ), 1, fp );
			fwrite( &PrimBulls[ i ].TimeInterval, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].TimeCount, sizeof( float ), 1, fp );
			fwrite( &PrimBulls[ i ].FirePoint, sizeof( int16 ), 1, fp );
			fwrite( &PrimBulls[ i ].SpotFX, sizeof( int16 ), 1, fp );
			i = PrimBulls[ i ].Prev;
		}

		i = FirstPrimBullFree;

		while( i != (uint16) -1 )
		{
			fwrite( &PrimBulls[ i ].Next, sizeof( PrimBulls[ i ].Next ), 1, fp );
			i = PrimBulls[ i ].Next;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Load PrimBulls Array & Connected Global Variables
	Input		:	FILE	*	File Pointer
	Output		:	FILE	*	Updated File Pointer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
FILE * LoadPrimBulls( FILE * fp )
{
	uint16	i;
	uint16	TempIndex;

	if( fp )
	{
		for( i = 0; i < MAXPRIMARYWEAPONS; i++ )
		{
			fread( &PrimaryWeaponsGot[ i ], sizeof( int16 ), 1, fp );
		}

		fread( &FirstPrimBullUsed, sizeof( FirstPrimBullUsed ), 1, fp );
		fread( &FirstPrimBullFree, sizeof( FirstPrimBullFree ), 1, fp );
		fread( &PrimaryFireDelay, sizeof( PrimaryFireDelay ), 1, fp );
		fread( &OrbitFireDelay, sizeof( OrbitFireDelay ), 1, fp );
		fread( &GeneralAmmo, sizeof( GeneralAmmo ), 1, fp );
		fread( &SussGunAmmo, sizeof( SussGunAmmo ), 1, fp );
		fread( &PyroliteAmmo, sizeof( PyroliteAmmo ), 1, fp );
		fread( &GeneralAmmoUsed, sizeof( GeneralAmmoUsed ), 1, fp );
		fread( &SussGunAmmoUsed, sizeof( SussGunAmmoUsed ), 1, fp );
		fread( &PyroliteAmmoUsed, sizeof( PyroliteAmmoUsed ), 1, fp );
		fread( &LaserTemperature, sizeof( LaserTemperature ), 1, fp );
		fread( &LaserOverheated, sizeof( LaserOverheated ), 1, fp );
		fread( &PowerLevel, sizeof( PowerLevel ), 1, fp );
		fread( &Toggle, sizeof( Toggle ), 1, fp );
		fread( &GlobalPrimBullsID, sizeof( GlobalPrimBullsID ), 1, fp );
		
		i = FirstPrimBullUsed;

		while( i != (uint16) -1 )
		{
			fread( &PrimBulls[ i ].Used, sizeof( BOOL ), 1, fp );
			fread( &PrimBulls[ i ].Next, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Prev, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Type, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].OwnerType, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Owner, sizeof( uint16 ), 1, fp );

			fread( &TempIndex, sizeof( TempIndex ), 1, fp );
			if( TempIndex != (uint16) -1 )
			{
				PrimBulls[ i ].EnemyGun = ( Enemies[ PrimBulls[ i ].Owner ].Object.FirstGun + TempIndex );
			}
			else
			{
				PrimBulls[ i ].EnemyGun = NULL;
			}

			fread( &PrimBulls[ i ].ID, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Weapon, sizeof( int8 ), 1, fp );
			fread( &PrimBulls[ i ].PowerLevel, sizeof( int16 ), 1, fp );
			fread( &PrimBulls[ i ].TrojPower, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].LifeCount, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].Speed, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].ColRadius, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].ColType, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Offset, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].Pos, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].Dir, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].LocalDir, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].UpVector, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].ColStart, sizeof( VECTOR ), 1, fp );
			fread( &PrimBulls[ i ].ColDist, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].ColFlag, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].ColPoint, sizeof( VERT ), 1, fp );
			fread( &PrimBulls[ i ].ColPointNormal, sizeof( NORMAL ), 1, fp );
			fread( &PrimBulls[ i ].ColGroup, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].GroupImIn, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].Mat, sizeof( MATRIX ), 1, fp );
			fread( &PrimBulls[ i ].line, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].fmpoly, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].numfmpolys, sizeof( int16 ), 1, fp );
			fread( &PrimBulls[ i ].poly, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].numpolys, sizeof( int16 ), 1, fp );
			fread( &PrimBulls[ i ].light, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].lightsize, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].r, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].g, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].b, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].Bounces, sizeof( uint16 ), 1, fp );
			fread( &PrimBulls[ i ].TimeInterval, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].TimeCount, sizeof( float ), 1, fp );
			fread( &PrimBulls[ i ].FirePoint, sizeof( int16 ), 1, fp );
			fread( &PrimBulls[ i ].SpotFX, sizeof( int16 ), 1, fp );
			i = PrimBulls[ i ].Prev;
		}

		i = FirstPrimBullFree;

		while( i != (uint16) -1 )
		{
			memset( &PrimBulls[i], 0, sizeof( PRIMARYWEAPONBULLET ) );
			PrimBulls[i].Used = FALSE;
			PrimBulls[i].Prev = (uint16) -1;
			PrimBulls[i].Type = (uint16) -1;
			PrimBulls[i].Owner = (uint16) -1;
			PrimBulls[i].GroupImIn = (uint16) -1;
			PrimBulls[i].fmpoly = (uint16) -1;
			PrimBulls[i].light = (uint16) -1;
			PrimBulls[i].line = (uint16) -1;
			PrimBulls[i].TimeInterval = (float) ( 11 - TrailDetailSlider.value );

			fread( &PrimBulls[ i ].Next, sizeof( PrimBulls[ i ].Next ), 1, fp );
			i = PrimBulls[ i ].Next;
		}
	}

	return( fp );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Next Valid Primary Weapon
	Input		:	Nothing
	Output		:	uint8	Primary Weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 GetNextValidPrimaryWeapon( void )
{
	float	AmmoLevels[ MAXPRIMARYWEAPONS ];
	uint8	StartWeapon;
	uint8	TempWeapon;

	AmmoLevels[ PULSAR ] = GeneralAmmo;
	AmmoLevels[ TROJAX ] = GeneralAmmo;
	AmmoLevels[ PYROLITE_RIFLE ] = PyroliteAmmo;
	AmmoLevels[ TRANSPULSE_CANNON ] = GeneralAmmo;
	AmmoLevels[ SUSS_GUN ] = SussGunAmmo;
	AmmoLevels[ LASER ] = GeneralAmmo;

	StartWeapon = Ships[ WhoIAm ].Primary;
	TempWeapon = StartWeapon;

	TempWeapon++;
	if( TempWeapon >= MAX_PRIMARY_WEAPONS ) TempWeapon = 0;

	while( ( !PrimaryWeaponsGot[ TempWeapon ] || !AmmoLevels[ TempWeapon ] ) &&
			( StartWeapon != TempWeapon ) )
	{
		TempWeapon++;
		if( TempWeapon >= MAX_PRIMARY_WEAPONS ) TempWeapon = 0;
	}

	return( TempWeapon );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Prev Valid Primary Weapon
	Input		:	Nothing
	Output		:	uint8	Primary Weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 GetPrevValidPrimaryWeapon( void )
{
	float	AmmoLevels[ MAXPRIMARYWEAPONS ];
	uint8	StartWeapon;
	uint8	TempWeapon;

	AmmoLevels[ PULSAR ] = GeneralAmmo;
	AmmoLevels[ TROJAX ] = GeneralAmmo;
	AmmoLevels[ PYROLITE_RIFLE ] = PyroliteAmmo;
	AmmoLevels[ TRANSPULSE_CANNON ] = GeneralAmmo;
	AmmoLevels[ SUSS_GUN ] = SussGunAmmo;
	AmmoLevels[ LASER ] = GeneralAmmo;

	StartWeapon = Ships[ WhoIAm ].Primary;
	TempWeapon = StartWeapon;

	if( TempWeapon == 0 ) TempWeapon = ( MAX_PRIMARY_WEAPONS - 1 );
	else TempWeapon--;

	while( ( !PrimaryWeaponsGot[ TempWeapon ] || !AmmoLevels[ TempWeapon ] ) &&
			( StartWeapon != TempWeapon ) )
	{
		if( TempWeapon == 0 ) TempWeapon = ( MAX_PRIMARY_WEAPONS - 1 );
		else TempWeapon--;
	}

	return( TempWeapon );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Conv Primary Weapon number from cheat to norm
	Input		:	int8	Primary Weapon
	Output		:	uint8	Primary Weapon ( Valid One )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int8 ConvPrimToNormWeapon( int8 Primary )
{
	int8	NewPrimary;

	NewPrimary = Primary;

	switch( Primary )
	{
		case NME_POWERLASER:
		case NME_LIGHTNING:
			NewPrimary = LASER;
			break;

		default:
			break;
	}

	return( NewPrimary );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Toggle Primary Weapon Type
	Input		:	int8	Primary Weapon
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void TogglePrimaryWeapons( int8 Primary )
{
	switch( PrimaryToFireLookup[ Primary ] )
	{
		case LASER:
			PrimaryToFireLookup[ Primary ] = NME_LIGHTNING;
			break;
		case NME_LIGHTNING:
			PrimaryToFireLookup[ Primary ] = NME_POWERLASER;
			break;
		case NME_POWERLASER:
			PrimaryToFireLookup[ Primary ] = LASER;
			break;
		default:
			break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Kill Primary bullets assight to
				:	OwnerType and Owner
	Input		:	uint16	OwnerType
				:	uint16	Owner
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void KillPrimBullsByOwner(  uint16 OwnerType, uint16 Owner )
{
	uint16	i, nextprim;

	i = FirstPrimBullUsed;

	while( i != (uint16) -1 )
	{
		nextprim = PrimBulls[i].Prev;
				
		if( ( OwnerType == PrimBulls[i].OwnerType ) && ( Owner == PrimBulls[i].Owner ) )
		{
			CleanUpPrimBull( i, TRUE );
		}

		i = nextprim;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get Ammo Usage for primary weapons
	Input		:	int16		Ship
				:	int8		Weapon
	Output		:	float		Ammo Usage
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
float GetPrimaryAmmoUsage( int16 Ship, int8 Weapon )
{
	float	Ammo = 0.0F;

	switch( Weapon )
	{
		case PULSAR:
			Ammo += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ];
			if( Ships[ Ship ].Object.PowerLevel >= PULSAR2SHOTPOWERLEVEL )
			{
				Ammo *= 2.0F;
			}
			break;
	
		case TROJAX:								/* Fire on release or when max reached */
			break;
	
		case PYROLITE_RIFLE:
			Ammo += ( PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ] * 3.0F );
			break;
	
		case TRANSPULSE_CANNON:
			Ammo += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ];
			break;
	
		case SUSS_GUN:
			Ammo += ( PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ] * 4.0F );
			break;
	
		case LASER:
			Ammo += ( PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ] * 2.0F );
			break;

		case NME_LIGHTNING:
			Ammo += ( PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ] * 2.0F );
			break;

		case NME_POWERLASER:
			Ammo += PrimaryWeaponAttribs[ Weapon ].AmmoUsage[ Ships[ Ship ].Object.PowerLevel ];
			break;
	
		default:
			break;
	}

	return( Ammo );
}

#ifdef OPT_ON
#pragma optimize( "", off )
#endif
