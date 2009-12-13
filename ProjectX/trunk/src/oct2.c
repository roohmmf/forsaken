#define INSIDE_BSP // disable to use bounding box inside check instead
#define BSP_ONLY

#include "main.h"
#include "new3d.h"
#include "quat.h"
#include "compobjects.h"
#include "bgobjects.h"
#include "object.h"
#include "node.h"
#include "networking.h"
#include "2dtextures.h"
#include "mload.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"
#include "ships.h"
#include "collision.h"
#include "2dpolys.h"
#include "lines.h"
#include "polys.h"
#include "lights.h"
#include "models.h"
#include "2dtextures.h"
#include "visi.h"
#include "tload.h"
#include "sfx.h"
#include "transexe.h"
#include "text.h"
#include "mxaload.h"
#include "camera.h"
#include "title.h"
#include "screenpolys.h"
#include "controls.h"
#include "net.h"
#include "triggers.h"
#include "enemies.h"
#include "bsp.h"
#include "trigarea.h"
#include "multiplayer.h"
#include "file.h"
#include "ai.h"
#include "water.h"
#include "spotfx.h"
#include "extforce.h"
#include "teleport.h"
#include "rtlight.h"
#include "restart.h"
#include "local.h"
#include "goal.h"
#include "loadsave.h"
#include "xmem.h"
#include "stats.h"
#include "timer.h"
#include "demo.h"
#include "file.h"
#include "singleplayer.h"
#include "render.h"
#include "input.h"

#ifdef SHADOWTEST
#include "triangles.h"
#include "shadows.h"
#endif

extern render_info_t render_info;
extern BOOL Bsp_Duplicate( BSP_HEADER *src, BSP_HEADER *dup );
extern BOOL Bsp_Identical( BSP_HEADER *b1, BSP_HEADER *b2 );
BSP_HEADER Bsp_Original;

extern int    BountyInLevel;
extern int    FlagsInLevel;
extern int    TeamFlagsInLevel[ MAX_TEAMS ];
extern int    TeamFlagsInShips[ MAX_TEAMS ];

extern void input_grab( BOOL clip );
extern BOOL NeedFlagAtHome;
extern BOOL OwnFlagTeleportsHome;
extern BOOL CanCarryOwnFlag;

BOOL OriginalLevels = FALSE;
BOOL CheatsDisabled = FALSE;
BOOL WaitingToQuit;

void ConfigureSpaceorbAxis( int joystick );
void DefaultJoystickSettings( USERCONFIG *u );

extern const char last_compiled[];
extern  float Countdown_Float;

extern  uint32 RecPacketSize;
extern  uint32 MaxRecPacketSize;

extern  uint32 CurrentBytesPerSecRec;
extern  uint32 CurrentBytesPerSecSent;
extern  uint32 MaxCurrentBytesPerSecRec;
extern  uint32 MaxCurrentBytesPerSecSent;


extern char *JoystickPOVDirections[];
extern VECTOR BikePos;

extern BOOL CTF;

extern uint8 QuickStart;

extern char *TeamName[ MAX_TEAMS ];
extern int16 DummyTextureIndex;

extern void Printint16( int16 num , int x , int y , int col );

extern void InitIndirectVisible( uint16 g );

extern void AddIndirectVisible( uint16 g );
extern  BOOL  RearCameraDisable;
extern  MODELNAME   SplashModelNames[MAXMODELHEADERS];
extern USERCONFIG *player_config;
extern char biker_config[];

extern MODELNAME *TitleModelSet;
extern float LevelTimeTaken;

extern BYTE PreDemoEndMyGameStatus;
extern  TEXT  DemoGameName;

extern  BOOL FullRearView;
extern  BOOL ShowStats;

extern  int AckMsgsActiveMax;
extern  int AckMsgsActive;

extern MENU MENU_Controls;
extern MENU MENU_Options;
extern MENU MENU_LoadSavedGame;
extern MENU MENU_SaveGame;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_BetweenLevels;
extern MENU MENU_NEW_NumberOfCrystals;
extern MENU MENU_NEW_WatchTeamSelect;

extern float VDUoffsetX;
extern float VDUoffsetY;
extern  int16 NextworkOldBikeNum;

extern  size_t  MemUsed;

#ifdef DEBUG_ON
extern uint32 CurrentBikeCompSpeech;
#endif

BOOL Cheated = FALSE;

void InitSoundInfo( MLOADHEADER * Mloadheader );
void InitShipSpeeds( void );

#define SAVESCREEN_3DFX

uint32        AnimOncePerFrame = 0;         // used for stuff that is displayed more than once in a single frame..

extern BOOL bSoundEnabled;

extern TeamCol[];
extern int Num_StatsMessage_Parts;
extern LIST BikeList;
extern LIST LevelList;

extern  BOOL ResetKillsPerLevel;

extern int  outside_map;

extern  int16 NextNewModel;
extern MENUSTATE MenuState;
extern  BOOL  ShowEFZones;
extern  BOOL  ShowTeleports;
extern BOOL MenuFrozen;
extern  int16 MaxKills;
extern  int16 NumGoldBars;
extern BOOL ReloadSfx;
extern float ReloadSfxPause;
extern  BOOL  IllegalTime;
extern  int   SinglePlayerTimeLimit;
extern  BOOL  BilinearSolidScrPolys;
extern  uint16  RandomStartPosModify;
extern  char * StatusTab[256];

extern BOOL ShowMessages; // show long list of chat messages
extern BOOL ShowStatistics; // show in-game statistics

BYTE  PreSynchupStatus;
char *CurrentLevelsList;

px_timer_t level_time;

void InitFontTransTable();

void InitModeCase(void);
void DebugState( const char * str );


// Ai Modes
char * AiModes[] = {
  "AIMODE_PLAYER",          // Controlled by PLAYER
  "AIMODE_FOLLOWPATH",        // Follow node path
  "AIMODE_IDLE",          // Do nothing for a while
  "AIMODE_SCAN",          // Look around for a target
  "AIMODE_MOVETOTARGET",      // Move towards selected target   
  "AIMODE_FIRE",      // Move towards selected target   
  "AIMODE_DOGFIGHT",        // Dogfight with the selected target   
  "AIMODE_KILLMINE",        // Attack And kill a Mine....
  "AIMODE_RETREAT",         // Try and get away from PLAYER
  "AIMODE_FORMATION",       // Stay in Formation and attack whan necessary...

  "AIMODE_DEATH_CRASHNBURN",    // Fall to the ground and Shatter...
  "AIMODE_DEATH_PINGOFF",     // Ping off in the dir of the bullet that killed me spinning as I go...
  "AIMODE_DEATH_PSYCHO",      // Twist and turn and shoot and explode...
  "",
  "",
  "",
};

extern  int EnemiesActive;
void CheckTimeLimit( void );
extern  int16 InGameLoadGameLevelNum;

BOOL  ScoreDisplaySfx = TRUE;
BOOL  IMustQuit = FALSE;
// Capture The Flag Stuff
BOOL  CaptureTheFlag	= FALSE;
BOOL  IHaveAFlag			= FALSE;
// End of Capture The Flag

// Bounty Hunt
BOOL  BountyHunt = FALSE;

static char fname[256];
static int fnum = 0;
extern int CameraStatus;  

char *  InitViewMessages[] = {
     "Loading OffScreen Surfaces" ,
     "Texture Load Prep         " ,
     "Loading Textures          " ,
     "Loading Models            " ,
     "Loading World Mesh        " ,
     "Loading Collision Skins   " ,
     "Init Sound Info           " ,
     "Init Ambient Lighting     " ,
     "Loading Sfx               " ,
     "Everything Else...Nodes   " ,
     "" 
};

extern  float MaxMoveSpeed;
extern  float MoveAccell;
extern  float MoveDecell;
extern  float MaxTurboSpeed;
extern  float TurboAccell;
extern  float TurboDecell;
extern  float MaxTurnSpeed;
extern  float TurnAccell;
extern  float TurnDecell;
extern  float MaxRollSpeed;
extern  float RollAccell;
extern  float RollDecell;
extern  float MaxBankAngle;
extern  float BankAccell;
extern  float BankDecell;

extern BOOL BrightShips;

extern  float LastDistance[MAX_SFX];

extern int GameCompleted;
extern  OFF_FILES OffsetFiles[];
extern  OFF_FILES Title_OffsetFiles[];
extern  OFF_FILES Splash_OffsetFiles[];

extern  int   DifficultyLevel;

extern int outside_group;

extern  BOOL  ScreenSaving;
extern  float WhiteOut;
extern float mouse_dx;
extern float mouse_dy;

void	MessageQuePrintAll( void );

BOOL  InsideResizeViewport = FALSE;

DWORD CurrentSrcBlend;
DWORD CurrentDestBlend;
DWORD CurrentTextureBlend;
 
#ifdef DEMO_SUPPORT
LONGLONG  GameStartedTime;    // when the game started
LONGLONG  GameElapsedTime;    // Real how long the game has been going in game time not real..
LONGLONG  TempGameElapsedTime;  // Real how long the game has been going in game time not real..
LONGLONG  GameCurrentTime;    // How long the game has been going...
LONGLONG  TimeDiff;
LONGLONG  Freq;
#endif

BOOL  JustExitedMenu =FALSE;

BOOL  Inside;
int   Depth;

void CalculateFramelag( void );

MENU  MENU_EditMacro1;
MENU  MENU_EditMacro2;
MENU  MENU_EditMacro3;

extern  FILE  * DemoFp;
extern  FILE  * DemoFpClean;
extern  BOOL  PlayDemo;
extern  BOOL  PauseDemo;
extern  BOOL  RecordDemo;
extern  SLIDER  DemoSpeed;
extern  SLIDER  DemoEyesSelect;
extern  BOOL  ShowWeaponKills;
extern  BOOL ShowStats; 

extern  uint16  IsGroupVisible[MAXGROUPS];

extern  MXLOADHEADER ModelHeaders[MAXMODELHEADERS];
extern  MODELNAME TitleModelNames[MAXMODELHEADERS]; 

extern  char  TitleNames[8][64];  

extern  ENEMY * FirstEnemyUsed;

extern  CAMERA  CurrentCamera;
extern  CAMERA  MainCamera;     // the main viewing screen...

float FPS = 0.0F;         // Frames Per Second...
//double  TPS = 0.0;          // Textures Per Second...     

#define MYTIMER
#undef MYTIMER

#define LEVELSFILE      "levels.dat"

extern  MENU  *       GetPlayerNumMenu;


#define FOV_GROW(A)     ((A) + 10.0F)
#define FOV_SHRINK(A)   ((A) - 10.0F)

#ifdef DEMO_SUPPORT
LONGLONG  DemoStartedTime;    // when the game started
LONGLONG  DemoEndedTime;      // when the game started
float   DemoTotalTime = 0.0F; // total game time (in seconds)
int32   DemoGameLoops = 0;
float DemoAvgFps = 0.0F;
extern  LONGLONG  DemoTimeSoFar;
#endif

#define MIN_VIEWPORT_WIDTH  (64)
#define MIN_VIEWPORT_HEIGHT (64)

#define VIEWPORT_GROW   (1.1F)
#define VIEWPORT_SHRINK   (1.0F/1.1F)

extern  TLOADHEADER Tloadheader;
extern  int16   SecondaryAmmo[ MAXSECONDARYWEAPONS ];
BOOL LockOutWindows = TRUE;

extern uint16 OnceOnly;
extern char         MyName[ 32 ];
extern  BOOL                    IsHost;
extern float  MessageTime[MAX_MESSAGES];
extern  char MessageBank[MAX_MESSAGES][200];
extern  int CurrentMessage;
extern  BYTE          MyGameStatus;
BYTE PreWaitingToSendMessagesStatus;
extern  BYTE          GameStatus[]; // Game Status for every Ship...
extern  BYTE          OverallGameStatus;      // what the host says he is doing...
//extern  LPDIRECT3DEXECUTEBUFFER Skin_Execs[ MAXGROUPS ];
//extern  LPDIRECT3DEXECUTEBUFFER Portal_Execs[ MAXGROUPS ];
extern RENDEROBJECT Skin_Execs[ MAXGROUPS ];
extern RENDEROBJECT	Portal_Execs[ MAXGROUPS ];
extern  float PyroliteAmmo;
extern  float SussGunAmmo;
extern  float GeneralAmmo;
extern  SHIPCONTROL control;
extern  REGENPOINT  * RegenPoints;
extern  int16     NumRegenPoints;
extern  float PowerLevel;
extern  float LaserTemperature;
extern  float NitroFuel;
extern  SECONDARYWEAPONBULLET SecBulls[MAXSECONDARYWEAPONBULLETS];
extern  ENEMY * TestEnemy;

extern  char  biker_name[256];
extern  int16 SelectedBike;

extern  float cral;
extern  int   HullHit;
extern  int   ShieldHit;
extern  BOOL  TargetComputerOn;

extern  int16 NumPickupsPerGroup[ MAXGROUPS ];
extern  PICKUP  Pickups[ MAXPICKUPS ];

extern  BOOL  DebugInfo;
extern  BOOL  GodMode;
extern  BOOL  GodModeOnceOnly;

extern  BOOL  TeamGame;
extern  BYTE  TeamNumber[MAX_PLAYERS];

extern  int   no_collision;   // disables player ship-to-background collisions
extern  SLIDER  TimeLimit;
extern  BOOL  CountDownOn;
extern  REGENPOINT  * RegenSlotsCopy[ MAX_PLAYERS ];

extern  BOOL  ShowTrigZones;
extern  BOOL  ShowColZones;
extern  BOOL  NodeCube;
extern  BOOL  OldNodeCube;
extern  MATRIX  MATRIX_Identity;

extern  uint32  TotalFmPolysInUse;
extern  uint32  TotalPolysInUse;
extern  uint32  TotalScrPolysInUse;
extern  MODELNAME ModelNames[MAXMODELHEADERS];
extern  BOOL  quitting; 
extern  BOOL  ShowBoundingBoxes;

BOOL InitViewport( void );


BYTE  InitView_MyGameStatus;
BYTE  ChangeLevel_MyGameStatus;

//LPDIRECTDRAWPALETTE ddpal;

void ProcessGameKeys( void );


float GetPlayerNumCount1 = 0.0F;
float GetPlayerNumCount2 = 0.0F;
int   GetPlayerNumCount = 0;

px_timer_t framelag_timer;

int i, j; // index counters

BOOL RearCameraActive;
int MissileCameraActive = 0;
uint16  CameraMissile = (uint16) -1;
extern BOOL MissileCameraEnable;
BYTE  TempMissileCam;

extern  REMOTECAMERA * ActiveRemoteCamera;

BOOL
Our_CalculateFrameRate(void);
BOOL VduFinished( MENU *Menu );
BOOL WriteMessage(const char *format, ...); // printf-a-like for bottom line of window

BOOL ScoreDisplay();
BOOL InitScoreDisplay();
BOOL StatsDisplay();
BOOL InitStatsDisplay();
BOOL ScoreDisplayOrig(void);

typedef enum {
	NO_BG,
	FULL_BG,
	BOX_BG
} stats_mode_t;

void ShowDetailedStats(int NumActivePlayers, BOOL TeamsGame, BOOL KillsBased, BOOL DetailedStats);
void ShowBasicStats(int NumActivePlayers);
void ShowInGameStats();
void ShowDeathModeStats();
void ShowGameStats(stats_mode_t mode);

int Secrets = 0;
int TotalSecrets = 0;
int CrystalsFound = 0;
extern  int16   NumInitEnemies;
extern  int16   NumKilledEnemies;

BOOL  ResizeViewport( void );
BOOL  FullScreenViewport();
BOOL  ClearBuffers( void );
BOOL  ClearZBuffer( void );

BOOL RenderCurrentCamera( void );

void  PlotSimplePanel( void );

render_viewport_t viewport;
render_viewport_t oldviewport;
int initfov = 0;
float viewplane_distance;
float hfov = START_FOV;
float chosen_fov = START_FOV;
float normal_fov = START_FOV;

BOOL  DrawPanel = FALSE;
BOOL  ReMakeSimplePanel = TRUE;

BOOL  OldDrawPanel = TRUE;
BOOL  Panel = TRUE;

BOOL ChangeLevel( void );
void SelectQuitCurrentGame( MENUITEM *Item );

float pixel_aspect_ratio;
float Oldframelag;  
float framelag = 0.0F; 
float real_framelag = 0.0F;
float Demoframelag = 0.5F;

px_timer_t level_timer;

extern int FontWidth;
extern int FontHeight;
extern int FontSourceWidth;
extern int FontSourceHeight;

extern  int PlayerSort[MAX_PLAYERS];
extern int16 NumOfActivePlayers;

int16   LevelNum = 0 ;
int16   NewLevelNum = 0 ;
int16   NumLevels = 0;
char  ShortLevelNames[MAXLEVELS][32];
char  LevelNames[MAXLEVELS][128];
char  CollisionNames[MAXLEVELS][128];
char  CollisionZNames[MAXLEVELS][128];
char  BspNames[MAXLEVELS][128];
char  BspZNames[MAXLEVELS][128];
char  ZoneNames[MAXLEVELS][128];
char  WaterNames[MAXLEVELS][128];
char  TextNames[MAXLEVELS][128];
char  MsgNames[MAXLEVELS][128];
char  ExternalForceNames[MAXLEVELS][128];
char  TeleportNames[MAXLEVELS][128];

char MissionTextNames[MAXLEVELS][128];
char MissionTextPics[MAXLEVELS][128];
char level_names[MAXLEVELS][128];

int16 CameraRendering = CAMRENDERING_None;

BOOL SeriousError = FALSE;
BOOL DoClipping = TRUE;
BOOL OnceOnlyChangeLevel = FALSE;

//bjdLPDIRECT3DEXECUTEBUFFER RenderBufs[ 2 ] = { NULL, NULL };
RENDEROBJECT RenderBufs[4];

void InitRenderBufs(/*LPDIRECT3DDEVICE lpDev*/ ); // bjd
void ReleaseRenderBufs( void );
//BOOL ChangeBackgroundColour( float R, float G, float B );
BOOL SetMatrixViewPort( void );
void PrintInitViewStatus( BYTE Status );

void UpdateBGObjectsClipGroup( CAMERA * Camera );
void UpdateEnemiesClipGroup( CAMERA * Camera  );
void SpecialDestroyGame( void );

extern  int16 PrimaryInLevel[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryInPlayers[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryInPlayers[ MAXSECONDARYWEAPONS ];
extern  int16 PrimaryToGenerate[ MAXPRIMARYWEAPONS ];
extern  int16 SecondaryToGenerate[ MAXSECONDARYWEAPONS ];
extern  int16 MinesInLevel[ MAXSECONDARYWEAPONS ];
extern  int16 OrbsInLevel;
extern  int16 OrbsToGenerate;
extern  int16 OrbsInPlayers;

// if met max kill limit, sets flag to change level
void CheckMetKillLimit();

// message colours (Title.c)
extern int KillMessageColour; 
extern int SystemMessageColour;
extern int FlagMessageColour;

// watch mode select player (Title.c)
extern SLIDER WatchPlayerSelect;

// (Ships.c)
extern int SwitchedToWatchMode;
extern int GetName(int Player);

// (stats.c)
extern void ScoreSort();

/*===================================================================
    Weapon Names...
===================================================================*/
char PrimaryNames[7][16] = {
            { "Pulsar" } ,
            { "Trojax" } ,
            { "Pyrolite" } ,
            { "Transpulse" } ,
            { "Suss Gun" } ,
            { "Laser" } ,
            { "" }  
};
char SecondaryNames[12][16] = {
            { "Mug" } ,
            { "Solaris" } ,
            { "Thief" } ,
            { "Scatter" } ,
            { "Gravgon" } , 
            { "MFRL" } , 
            { "Titan" } ,
            { "Purge Mine" } ,
            { "Pine Mine" } ,
            { "Quantum Mine" } ,
            { "Spider Mine" } ,
            { "" }  

};
int PrimaryLengths[7] ={ 
             sizeof("Puls")-1  ,
             sizeof("Troj")-1  ,
             sizeof("Pyro")-1  ,
             sizeof("Tran")-1  ,
             sizeof("Suss")-1  ,
             sizeof("Beam")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug ")-1 ,
             sizeof("Sola")-1 ,
             sizeof("Thei")-1 ,
             sizeof("Scat")-1 ,
             sizeof("Grav")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Tita"  )-1 ,
             sizeof("Purg")-1 ,
             sizeof("Pine")-1 ,
             sizeof("Quan")-1,
             sizeof("Spid")-1 };
#if 0
int PrimaryLengths[7] ={ 
             sizeof("Pulsar")-1  ,
             sizeof("Trojax")-1  ,
             sizeof("Pyrolite")-1  ,
             sizeof("Transpulse")-1  ,
             sizeof("Suss Gun")-1  ,
             sizeof("Beam Laser")-1 };
int SecondaryLengths[12] = {
             sizeof("Mug")-1 ,
             sizeof("Solaris")-1 ,
             sizeof("Thief")-1 ,
             sizeof("Scatter")-1 ,
             sizeof("Gravgon")-1 , 
             sizeof("MFRL")-1 , 
             sizeof("Titan")-1 ,
             sizeof("Purge Mine")-1 ,
             sizeof("Pine Mine")-1 ,
             sizeof("Quantum Mine")-1,
             sizeof("Spider Mine")-1 };
#endif


/*===================================================================
    Panel Description Stuff...
===================================================================*/

float ModeScaleX;
float ModeScaleY;
  
/*===================================================================
    Off Screen Sufaces...Used to Blit to screen...
===================================================================*/
RENDERMATERIAL *lpBmat;    // a Material for the Background clearing

MLOADHEADER  Mloadheader;
MCLOADHEADER MCloadheader;          //  inner skin collision map...
MCLOADHEADER MCloadheadert0;        //  0 thickness collision map...

//extern  LPDIRECT3DEXECUTEBUFFER Portal_lpD3DExBuf;

extern  uint16      FirstFmPolyUsed;
extern  uint16      FirstPolyUsed;
extern  uint16      FirstLineUsed;
extern  uint16      FirstScrPolyUsed;
extern  GLOBALSHIP    Ships[MAX_PLAYERS+1];
extern  BYTE      WhoIAm;
extern  BYTE      Current_Camera_View;    // which object is currently using the camera view....
extern  BOOL      RemoteCameraActive;

extern  int16     ShowPortal;
extern  int16     ShowSkin;
extern  int16         NamesAreLegal;
extern  SHORTNAMETYPE     Names;  // all the players short Names....

BOOL MainGame(); // bjd

void Build_View();
BOOL DispTracker( void ); // bjd


/*===================================================================
  Global Variables
===================================================================*/   
#define Near  ( 100.0F * GLOBAL_SCALE )
//#define Far   ( 65535.0F * GLOBAL_SCALE )
#define Far   ( 49152.0F * GLOBAL_SCALE )
#define MinNear ( 50.0F * GLOBAL_SCALE )

RENDERMATRIX proj = {
    RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(Far/(Far-Near)), RENDERVAL(1.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(-Far*Near/(Far-Near)), RENDERVAL(0.0) };

RENDERMATRIX panelproj = {
    RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(Far/(Far-Near)), RENDERVAL(1.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(-Far*Near/(Far-Near)), RENDERVAL(0.0) };

RENDERMATRIX trackerproj = {
    RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(Far/(Far-Near)), RENDERVAL(1.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(-Far*Near/(Far-Near)), RENDERVAL(0.0) };

MATRIX  ProjMatrix = {
    RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(2.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(Far/(Far-Near)), RENDERVAL(1.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(-Far*Near/(Far-Near)), RENDERVAL(0.0) };


  VECTOR  SlideLeft = { -1.0F, 0.0F, 0.0F };
  VECTOR  SlideRight  = { 1.0F, 0.0F, 0.0F };
  VECTOR  SlideUp   = { 0.0F, 1.0F, 0.0F };
  VECTOR  SlideDown = { 0.0F, -1.0F, 0.0F };
  VECTOR  Forward   = { 0.0F, 0.0F, 1.0F };
  VECTOR  Backward  = { 0.0F, 0.0F,-1.0F };
  VECTOR  NullVector  = { 0.0F, 0.0F, 0.0F };

  VECTOR  Camera_Dir = { 0.0F, 0.0F,0.0F };

/*===================================================================
===================================================================*/
//static D3DEXECUTEDATA d3dExData;
//static D3DEXECUTEBUFFERDESC debDesc;

/*
 * Global projection, view, world and identity matricies
 */

RENDERMATRIX view;
RENDERMATRIX identity = {
    RENDERVAL(1.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(1.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(1.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(1.0)
};

RENDERMATRIX world = {
    RENDERVAL(1.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(1.0), RENDERVAL(0.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(1.0), RENDERVAL(0.0),
    RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(0.0), RENDERVAL(1.0)
};

extern BOOL StereoEnabled;
stereo_mode_t StereoMode;
extern float StereoEyeSep;
extern float StereoFocalDist;
extern stereo_right_color_t StereoRightColor;

void stereo_adjust( RENDERMATRIX *m )
{
	float left, right, offset;

	if( StereoMode != ST_CENTER )
		offset = 0.5f * StereoEyeSep / StereoFocalDist;
	else
		return;

	right = pixel_aspect_ratio * tanf( hfov * (float)M_PI_2/180.0f );
	left = -right;

	switch( StereoMode )
	{
	case ST_LEFT:
		left -= offset;
		right -= offset;
		break;
	case ST_RIGHT:
		left += offset;
		right += offset;
		break;
	}

	m->_31 = (right+left)/(right-left);
}

BOOL SetFOV( float fov )
{
	float screen_width, screen_height;
	float Scale, NewNear;

	if ( fov <= 1.0F || fov >= 150.0F )
		fov = hfov;

	if (!FSGetViewPort(&viewport))
	{
		Msg( "GetViewport failed.\n%s", render_error_description(0) );
		return FALSE;
	}

	if ( render_info.fullscreen )
	{
		screen_width = (float) render_info.ThisMode.w;
		screen_height = (float) render_info.ThisMode.h;
	}
	else
	{
		screen_width = (float) render_info.WindowsDisplay.w;
		screen_height = (float) render_info.WindowsDisplay.h;
	}

	pixel_aspect_ratio = render_info.aspect_ratio * screen_height / screen_width;
	viewplane_distance = (float) ( viewport.Width / ( 2 * tan( DEG2RAD( fov ) * 0.5 ) ) );
	proj._11 = 2 * viewplane_distance / viewport.Width;
	proj._22 = 2 * viewplane_distance / ( viewport.Height / pixel_aspect_ratio );

	if( fov > START_FOV )
	{
		Scale = ( ( MAX_FOV - fov ) / ( MAX_FOV - START_FOV ) );
		NewNear = ( MinNear + ( ( Near - MinNear ) * Scale ) );
		proj._33 = RENDERVAL(Far/(Far-NewNear));
		proj._34 = RENDERVAL( 1.0 );
		proj._43 = RENDERVAL(-Far*NewNear/(Far-NewNear));
		proj._44 = RENDERVAL( 0.0 );
	}
	else
	{
		proj._33 = RENDERVAL(Far/(Far-Near));
		proj._34 = RENDERVAL( 1.0 );
		proj._43 = RENDERVAL(-Far*Near/(Far-Near));
		proj._44 = RENDERVAL( 0.0 );
	}

	stereo_adjust(&proj);

	ProjMatrix._11 = proj._11;
	ProjMatrix._22 = proj._22;
	ProjMatrix._33 = proj._33;
	ProjMatrix._34 = proj._34;
	ProjMatrix._43 = proj._43;
	ProjMatrix._44 = proj._44;

	hfov = fov;

	if (!FSSetProjection(&proj))
		return FALSE;

	return TRUE;
}


void SetViewportError( char *where, render_viewport_t *vp )
{
  static char msg[1024];
  sprintf( msg,
	"SetViewport failed in %s.\n"
    "size=%d\n"
    "xpos=%d ypos=%d\n"
    "width=%d height=%d\n"
    "xscale=%f yscale=%f\n"
    //"xmax=%f ymax=%f\n"
    "zmin=%f zmax=%f\n",
    where,
    sizeof( *vp ),
    vp->X, vp->Y,
    vp->Width, vp->Height,
    vp->ScaleX, vp->ScaleY,
    //vp->dvMaxX, vp->dvMaxY,
    vp->MinZ, vp->MaxZ );
  Msg( msg );
}

// read KEYS.txt and DEUBG_KEYS.txt before changing.
void ProcessGameKeys( void )
{

  static int16 startpos = 0;

  // if were not (!) in (normal,single,attract or demo) modes
  // then don't listen to keys...
  if ( ! (
           ( MyGameStatus == STATUS_Normal       ) ||
           ( MyGameStatus == STATUS_SinglePlayer ) ||
           ( MyGameStatus == STATUS_PlayingDemo  )
     )   )
  {
    return;
  }

  // ESCAPE
  if ( buffered_key_released( SDLK_ESCAPE ) )
    // if were not currently in a menu
    //   ignore keystroke if we just exited the menu
    //   stops going back itno menu from hitting to many times
    if ( !CurrentMenu && !JustExitedMenu )
    {
      // if were playing a demo
      if( PlayDemo )
        // show the demo menu
        MenuRestart( &MENU_DemoPlaying );
      // if were playing the game
      else
	  {
			// show the multi-player menu
			if(GameStatus[WhoIAm] != STATUS_PlayingDemo && GameStatus[WhoIAm] != STATUS_SinglePlayer)
				 MenuRestart( &MENU_InGame );
			// show the single-player menu
			else
				 MenuRestart( &MENU_InGameSingle );
	  }

    // if were not in the menu
    // and the above check failed
    // reset variable so 3rd escape takes us back in
	}else{
      JustExitedMenu = FALSE;
	}


  // debuggin keys
  if ( DebugInfo ) {

    // Shift Modifier
    if ( SDL_GetModState() & KMOD_SHIFT )
    {

      // Shift + F1
      if ( buffered_key_released( SDLK_F1 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_DebugHelp );
        else if ( CurrentMenu == &MENU_DebugHelp )
          MenuExit();

      // Shift + Ctrl Modifier
      if ( SDL_GetModState() & KMOD_CTRL )
      {

        // Shift + Ctrl + F2
        if ( buffered_key_released( SDLK_F2 ) )
          SetFOV( chosen_fov = normal_fov );
        
        // Shift + Ctrl + F3
        if ( buffered_key_released( SDLK_F3 ) )
          SetFOV( chosen_fov = normal_fov );

      } // end shift + ctrl modifier
      else // shift only modifier
      {

        // Shift + F2
        if ( buffered_key_released( SDLK_F2 ) )
          SetFOV( chosen_fov = FOV_GROW( hfov ) );

        // Shift + F3
        if ( buffered_key_released( SDLK_F3 ) )
          SetFOV( chosen_fov = FOV_SHRINK( hfov ) );

      } // end shift only modifier

      // Shift + F4
      if ( buffered_key_released( SDLK_F4 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_GotoRoom );

      // Shift + F5
      if ( buffered_key_released( SDLK_F5 ) )
      {
        InitShipStartPos( WhoIAm, ++startpos );
        AddColourMessageToQue(SystemMessageColour,
          "You are now in %s\n",
          Mloadheader.Group[ Ships[ WhoIAm ].Object.Group ].name
        );
      }

      // Shift + F6
      if ( buffered_key_released( SDLK_F6 ) )
        DoClipping = !DoClipping;

      // Shift + F7
      if ( buffered_key_released( SDLK_F7 ) )
        ShowBoundingBoxes = !ShowBoundingBoxes;

      // Shift + F8
      if ( buffered_key_released( SDLK_F8 ) )
        ShowEFZones = !ShowEFZones;

      // Shift + F9
      if ( buffered_key_released( SDLK_F9 ) )
        if( ShowPortal == 4 )
          ShowPortal = 0;
        else
          ShowPortal++;

      // Shift + F10
      if ( buffered_key_released( SDLK_F10 ) )
        ShowColZones = !ShowColZones;

#ifdef POLYGONAL_COLLISIONS

      // Shift + F11
      if ( buffered_key_released( SDLK_F11 ) )
        if( ShowSkin == 6 )
          ShowSkin = 0;
        else
          ShowSkin++;

#endif

      // Shift + F12
      if( buffered_key_released( SDLK_F12 ) )
        no_collision = !no_collision;

    } // end shift modifier
    // CTRL Modifiers
    else if ( SDL_GetModState() & KMOD_CTRL )
    {

		// Ctrl + F1
		if ( buffered_key_released( SDLK_F1 ) )
			ShowTeleports = !ShowTeleports;

		// Ctrl + F2
		if ( buffered_key_released( SDLK_F2 ) )
			ShowTrigZones = !ShowTrigZones;

#ifndef POLYGONAL_COLLISIONS
#ifdef REMOTE_CAMERA_ENABLED
		// Ctrl + F3
		if ( buffered_key_released( SDLK_F3 ) )
			if( !RemoteCameraActive )
			{
				RemoteCameraActive = TRUE;
				Ships[MAX_PLAYERS].Object.Pos = Ships[WhoIAm].Object.Pos;
				Ships[MAX_PLAYERS].Object.Group = Ships[WhoIAm].Object.Group;
				Ships[MAX_PLAYERS].Object.Mode = NORMAL_MODE;
				Ships[MAX_PLAYERS].enable = 1;
				Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
			}else{
				RemoteCameraActive = FALSE;
				Ships[MAX_PLAYERS].enable = 0;
				Current_Camera_View = WhoIAm;   // which object is currently using the camera view....
			}
#endif
#endif

    } // end ctrl modifier
    else // no modifiers
    {

		// F8
		if ( buffered_key_released( SDLK_F8 ) )
			// Awesome !!!
			// We need to make this a standard feature!!!
			if( TargetComputerOn )
				TargetComputerOn = FALSE;
			else
				TargetComputerOn = TRUE;

    }

  } // end debug keys
  else // non debugging mode keys
  {

    // F1
    if ( buffered_key_released( SDLK_F1 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Controls );
      else if ( CurrentMenu == &MENU_Controls )
        MenuExit();

    // F2
    if ( buffered_key_released( SDLK_F2 )  )
      if ( !CurrentMenu )
        MenuRestart( &MENU_Options );
      else if ( CurrentMenu == &MENU_Options )
        MenuExit();

    // single player
    if ( MyGameStatus == STATUS_SinglePlayer )
    {

      // F3
      if ( buffered_key_released( SDLK_F3 ) )
        if ( !CurrentMenu )
          MenuRestart( &MENU_LoadSavedGame );
        else if ( CurrentMenu == &MENU_LoadSavedGame )
          MenuExit();
  
      // F4        
      if ( buffered_key_released( SDLK_F4 ) )
        // quick save
        if ( !CurrentMenu )
          MenuRestart( &MENU_SaveGame );
        else if ( CurrentMenu == &MENU_SaveGame )
          MenuExit();

    }

	// multiplayer
	else
	{
		// F3
		// F4
	}

    // F5
    if ( buffered_key_released( SDLK_F5 ) )
      MissileCameraEnable = !MissileCameraEnable;

	// F6
	if ( buffered_key_released( SDLK_F6 ) )
		RearCameraActive = !RearCameraActive;

	// F7
	if ( buffered_key_released( SDLK_F7 ) )
		Panel = !Panel;

    // single player mode
    if( MyGameStatus == STATUS_SinglePlayer )
    {

      // F9
      if ( buffered_key_released( SDLK_F9 ) )
        LastTextMessage();

      // F10
      if ( buffered_key_released( SDLK_F10 ) )
        NextTextMessage();

    } // end single player mode
    else // not single player mode
    {

      // Shift Modifier
      if ( SDL_GetModState() & KMOD_SHIFT )
      {

        // Shift + F9
        if ( buffered_key_released( SDLK_F9 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro1 );
          else if ( CurrentMenu == &MENU_EditMacro1 )
            MenuExit();

        // Shift + F10
        if ( buffered_key_released( SDLK_F10 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro2 );
          else if ( CurrentMenu == &MENU_EditMacro2 )
            MenuExit();

        // Shift + F11
        if ( buffered_key_released( SDLK_F11 ) )
          if ( !CurrentMenu )
            MenuRestart( &MENU_EditMacro3 );
          else if ( CurrentMenu == &MENU_EditMacro3 )
            MenuExit();

      } // Shift Modifier
      else // no modifier
      {

        // F9
        if ( buffered_key_released( SDLK_F9 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt1, 0);

        // F10
        if ( buffered_key_released( SDLK_F10 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt2, 0);

        // F11
        if ( buffered_key_released( SDLK_F11 ) )
          SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_Taunt3, 0);

      } // end (not) shift modifier
	} // end not single player
  } // end normal keys
} // ProcessGameKeys

BOOL
ResizeViewport( void )
{
	int left, top;
	int width, height;
	int maxwidth, maxheight;
	BOOL  NewDrawPanel;
	BOOL  NewDrawSimplePanel;
	float scale = 1.0F;
  
	InsideResizeViewport = TRUE;

	/*
     * Setup the viewport for specified viewing area
     */

	if (!FSGetViewPort(&viewport))
	{
        Msg( "GetViewport failed.\n%s", render_error_description(0) );
        return FALSE;
    }
	maxwidth = render_info.window_size.cx;

	NewDrawSimplePanel = FALSE;
  
	maxheight = render_info.window_size.cy;

	if ( scale < 1.01F )
	{
		width = (int) floor( maxwidth * scale );
		if ( width < MIN_VIEWPORT_WIDTH )
		{
			width = MIN_VIEWPORT_WIDTH;
			scale = (float) width / maxwidth;
		}
		height = (int) floor( maxheight * scale );

		NewDrawPanel = FALSE;

		left = ( ( maxwidth - width ) >> 1 ) & ~1;
		top = ( ( maxheight - height ) >> 1 ) & ~1;
	}
	else
	{
		width = maxwidth;
		height = maxheight;
		if ( scale > 1.11F )
		{
			NewDrawPanel = TRUE;
			scale = 1.2F;
		}
		else
		{
			NewDrawPanel = FALSE;
		}
		left = 0;
		top = 0;
	}

	// This disables the 3d panel...
	NewDrawPanel = DrawPanel;

	viewport.X = left;
	viewport.Y = top;
	viewport.Width = width;
	viewport.Height = height;
	viewport.ScaleX = viewport.Width / (float)2.0;
	viewport.ScaleY = viewport.Height / (float)2.0;

	if (!FSSetViewPort(&viewport))
	{
#ifdef DEBUG_VIEWPORT
    SetViewportError( "ResizeViewport", &viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return FALSE;
    }
  
	SetFOV( hfov );

	FSClearBlack();

	return TRUE;
}

BOOL FullScreenViewport()
{
	int left, top;
	int width, height;
	int maxwidth, maxheight;

	if (!FSGetViewPort(&viewport))
	{
        Msg( "GetViewport failed.\n%s", render_error_description(0) );
        return FALSE;
    }

	maxwidth = render_info.window_size.cx;
	maxheight = render_info.window_size.cy;
	width = maxwidth;
	height = maxheight;
	left = 0;
	top = 0;

	viewport.X = left;
	viewport.Y = top;
    viewport.Width = width;
    viewport.Height = height;
    viewport.ScaleX = viewport.Width / (float)2.0;
    viewport.ScaleY = viewport.Height / (float)2.0;
/* bjd 
    viewport.dvMaxX = (float)D3DDivide(RENDERVAL(viewport.dwWidth),
                                       RENDERVAL(2 * viewport.dvScaleX));
    viewport.dvMaxY = (float)D3DDivide(RENDERVAL(viewport.dwHeight),
                                       RENDERVAL(2 * viewport.dvScaleY));
*/

	if (!FSSetViewPort(&viewport))
	{
#ifdef DEBUG_VIEWPORT
    SetViewportError( "FullScreenViewport", &viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return FALSE;
    }
	SetFOV( hfov );
	FSClearBlack();
	return TRUE;
}

BOOL InitLevels( char * levels_list )
{

  FILE *f;
  char levels_file[ 256 ];

  if ( !levels_list ){
    levels_list = LEVELSFILE;
  }
  
  // path to data\\levels\\<type>.bat
  sprintf( levels_file, "data\\levels\\%s", levels_list );
  
  // set some globals
  CurrentLevelsList = levels_list;
  OriginalLevels = TRUE;

  /////
  // Read Levels From File
  /////

  f = file_open( levels_file, "r" );
  if (!f )
    return FALSE;

  // scan each level name into ShortLevelNames[j]
  j=0;
  while ( j < MAXLEVELS && fscanf( f, " %s", ShortLevelNames[ j ] ) == 1 )
  {


	// get the path to the current mxv file
	sprintf( LevelNames[ j ], "data\\levels\\%s\\%s.mxv", ShortLevelNames[ j ], ShortLevelNames[ j ] );

	// check to see if the level mxv file exists
	if ( !File_Exists( LevelNames[ j ] ) )
	 continue;

	// add paths for a level to the lists
	sprintf( CollisionNames[ j ],		"data\\levels\\%s\\%s.mc",    ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( CollisionZNames[ j ],		"data\\levels\\%s\\%sz.mc",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( BspNames[ j ],				"data\\levels\\%s\\%s.bsp",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( BspZNames[ j ],			"data\\levels\\%s\\%sz.bsp",  ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( ZoneNames[ j ],			"data\\levels\\%s\\%s.zon",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( WaterNames[ j ],			"data\\levels\\%s\\%s.wat",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( TextNames[ j ],			"data\\levels\\%s\\%s.txt",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MsgNames[ j ],				"data\\levels\\%s\\%s.msg",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( ExternalForceNames[ j ],	"data\\levels\\%s\\%s.gf",    ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( TeleportNames[ j ],		"data\\levels\\%s\\%s.tel",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MissionTextNames[ j ],		"data\\levels\\%s\\%s.mis",   ShortLevelNames[ j ], ShortLevelNames[ j ] );
	sprintf( MissionTextPics[ j ],		"data\\levels\\%s\\%s.ppm",   ShortLevelNames[ j ], ShortLevelNames[ j ] );

	// increment counter
	j++;

  }

  NumLevels = j;
  fclose( f );

  // no levels were found
  // this should never happen
  // unless you delete all the level directories
  // or the level file doesn't exist
  if( !NumLevels ) return FALSE;

  // select default level
  LevelList.items = 0;
  LevelList.top_item = 0;
  LevelList.display_items = 8;
  LevelList.selected_item = 0;  // you can only ever start game on first level!
  NewLevelNum = 0;

  // add each level to the master list
  for (j = 0; j < NumLevels; j++)
  {
    strcpy( (char*) &LevelList.item[LevelList.items][0] , (char*) &ShortLevelNames[j][0] );
    LevelList.items++;
  }

  // Read TXT in of default level
  LoadLevelText( NULL );

  // success !
  return TRUE;

}


BOOL SetMatrixViewPort( void )
{
	RENDERMATERIAL bmat;
	memset(&bmat, 0, sizeof(RENDERMATERIAL));

/* TODO - what did this function do in d3d6 ?
	if (lpView->lpVtbl->SetBackground(lpView, hBmat) != D3D_OK) {
		return FALSE;
	}
*/

#if 0 // bjd - TODO - CHECK
      STATE_DATA(D3DLIGHTSTATE_AMBIENT, RGBA_MAKE(255, 255, 255, 0), lpPointer);
#endif

	FSSetProjection(&proj);
	FSSetView(&identity);
	FSSetWorld(&world);

	world = identity;

	if (!FSGetViewPort(&viewport))
    {
      viewport.Width = 320;
      viewport.Height = 200;
    }

    if ( !initfov )
    {
      SetFOV( chosen_fov = normal_fov );
      initfov = 1;
    }

    if( InsideResizeViewport == FALSE )
    {
		InitViewport();
    }
	else
	{
		InsideResizeViewport = FALSE;
	}

  return TRUE;
}

float HealthCount = 0.0F;
int PowerSizes[6] = { 0 , 4 , 16 , 24 , 40 , 56 };
void DrawSimplePanel()
{
			static int max = 0;
			static int least = 10000000;
	int energy;

    if( WhoIAm == Current_Camera_View )
    {
		// Add Crosshair Polygon..
		AddScreenPolyText( (uint16) 63 , (float) (viewport.X + (viewport.Width>>1)) , (float) (viewport.Y + (viewport.Height>>1)) , 64, 255, 64, 255 );

		// trojax level
		energy = (int) ( ( PowerLevel * 0.01F ) * 9.0F );

		// or lazer temperature
		if( !energy )
			energy = (int) ( ( LaserTemperature *0.01F ) * 9.0F );

		// render trojax/lazer bar
		if( energy )
		{
			// 72   (first frame) is power bar charging
			// 72-8 (last frame)  is power bar at full charge
			if( energy > 8 ) energy = 8;
			AddScreenPolyText( (72-energy), (float) (viewport.X + (viewport.Width>>1))-16 , (float) (viewport.Y + (viewport.Height>>1))+4 , 64, 255, 64, 255 );
		}

		// nitro bar
		if ( ( control.turbo || Ships[WhoIAm].Object.CruiseControl == CRUISE_NITRO ) && NitroFuel )
		{
			AddScreenPolyTextScale( 72, (float) ( (render_info.window_size.cx>>1) - (NitroFuel - 8) ), (float) (viewport.Y + (viewport.Height>>1)-7 ) ,
			(float) ( ( ( 1.0F / 100.0F ) * ( NitroFuel * 0.5F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F, 64, 255, 64, 255 );
		}

		// character flags
		if( Ships[WhoIAm].Invul )
		{
			Print4x5Text( "I" , FontWidth , render_info.window_size.cy-((FontHeight*4)+8) , 2 );
			Printuint16( (uint16) (Ships[WhoIAm].InvulTimer / 60.0F) , FontWidth*4 , render_info.window_size.cy-((FontHeight*4)+8) , 2 );
		}
		if( Ships[WhoIAm].Object.Flags & SHIP_SuperNashram )
		{
			Print4x5Text( "S" , FontWidth , render_info.window_size.cy-((FontHeight*5)+10) , 2 );
			Printuint16( (uint16) (Ships[WhoIAm].SuperNashramTimer / 60.0F) , FontWidth*4 , render_info.window_size.cy-((FontHeight*5)+10) , 2 );
		}
		if( Ships[WhoIAm].Object.Flags & SHIP_Stealth )
		{
			Print4x5Text( "C" , FontWidth , render_info.window_size.cy-((FontHeight*6)+12) , 2 );
			Printuint16( (uint16) (Ships[WhoIAm].StealthTime / 60.0F) , FontWidth*4 , render_info.window_size.cy-((FontHeight*6)+12) , 2 );
		}
    }
    
    if( Panel && !PlayDemo )
    {
        if( !DrawPanel && (WhoIAm == Current_Camera_View ) ) 
        {
          // Full Screen Minimum Stats...

			// blt hull
			AddScreenPolyText( (uint16) 56 , (float) FontWidth , (float) (render_info.window_size.cy-((FontHeight*1)+2) ), 32, 255, 32, 255 );
			// blt shld
			AddScreenPolyText( (uint16) 55 , (float) FontWidth , (float) (render_info.window_size.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
			// Blt Primary
			AddScreenPolyText( (uint16)( Ships[WhoIAm].Primary + 38 ) , (float) (render_info.window_size.cx - ( FontWidth*6) - ( FontWidth*PrimaryLengths[Ships[WhoIAm].Primary] ) ), (float) (render_info.window_size.cy-((FontHeight*2)+4) ), 32, 255, 32, 255 );
			// Blt Secondary
			AddScreenPolyText( (uint16)( Ships[WhoIAm].Secondary + 44 ) , (float) (render_info.window_size.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[Ships[WhoIAm].Secondary] ) ) , (float) (render_info.window_size.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
			// Blt Power Pods
			AddScreenPolyText( (uint16) (Ships[WhoIAm].Object.PowerLevel + 57 ), (float) (render_info.window_size.cx >> 1) - ( ( FontWidth * 7) >>1 ) , (float) (render_info.window_size.cy-((FontHeight*1)+2)), 32, 255, 32, 255 );
			// Blt Mine..
			energy = (int) GetBestMine();
			if( energy != 65535 )
			{
				AddScreenPolyText( (uint16)( energy + 44 ) , (float) (render_info.window_size.cx - ( FontWidth*6) - ( FontWidth*SecondaryLengths[energy] ) ), (float) (FontHeight), 32, 255, 32, 255 );
				Printuint16( (uint16) SecondaryAmmo[energy] , render_info.window_size.cx - ( FontWidth*5) , FontHeight , 2 );
			}

			// poly shld bar
			AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (render_info.window_size.cy-(FontHeight*2)-2) ,
					(float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Shield *0.25F ) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
					(uint8)(63+(ShieldHit * (192/24) )), (uint8)(255-(ShieldHit * (192/24) )), 64, 255 );

			if( ShieldHit )
				ShieldHit -=1;

			// poly hull bar
			AddScreenPolyTextScale( 72, (float) (FontWidth*10)-4, (float) (render_info.window_size.cy-(FontHeight*1)-2) ,
					(float) ( ( ( 1.0F / 256.0F ) * ( Ships[WhoIAm].Object.Hull * 0.25F) ) * ( (32.0F-0.125F) + 0.125F ) ) , 1.0F,
					(uint8)(63+(HullHit * (192/24) )), (uint8)(255-(HullHit * (192/24) )), 64, 255 );

			if( HullHit )
				HullHit -=1;

			// blt shield num
			Printuint16( (uint16) Ships[WhoIAm].Object.Shield , FontWidth*6 , render_info.window_size.cy-((FontHeight*2)+4) , 2 );

			// blt hull num
			if( Ships[WhoIAm].Object.Hull > 0.0F && Ships[WhoIAm].Object.Hull < 1.0F )
				Printuint16( (uint16) 1 , FontWidth*6 , render_info.window_size.cy-((FontHeight*1)+2) , 2 );
			else
				Printuint16( (uint16) Ships[WhoIAm].Object.Hull , FontWidth*6 , render_info.window_size.cy-((FontHeight*1)+2) , 2 );

			// Blt Primary ammo
			if( Ships[WhoIAm].Primary == PYROLITE_RIFLE )
				energy =  (int) PyroliteAmmo;
			else
			{
				if( Ships[WhoIAm].Primary == SUSS_GUN )
					energy = (int) SussGunAmmo;
				else
					energy = (int) GeneralAmmo;
			}
			Printuint16( (uint16) energy , render_info.window_size.cx - ( FontWidth*5) , render_info.window_size.cy-((FontHeight*2)+4) , 2 );

			// Blt Secondary ammo
			Printuint16( (uint16) GetCurSecAmmo() , render_info.window_size.cx - ( FontWidth*5) , render_info.window_size.cy-((FontHeight*1)+2) , 2 );
        }
    
      if( (NamesAreLegal != 0) || IsHost )
      {
        ScoreSort();
        PrintScoreSort();
		CheckMetKillLimit();
      }
      
	  // show long message history
	  if(ShowMessages) 
		  MessageQuePrintAll();
	  // show last 3 recent messages
	  else
		MessageQuePrint();

	  // if we are dead and waiting for a game
	  // show statistics
	  if( Ships[ WhoIAm ].Object.Mode == LIMBO_MODE && !SwitchedToWatchMode )
	  {
		  ShowDeathModeStats();

	  // if we have show stats activated (ex: you pressed the stats button)
	  // show statistics
	  } else if ( ShowStatistics )
		  ShowInGameStats();

	  // watch mode
	  if(SwitchedToWatchMode)
	  {
			// show who i am watching
			CenterPrint4x5Text( (char *)GetName(WatchPlayerSelect.value), render_info.window_size.cy - 15, 4 );
			// display cross-hair
			AddScreenPolyText(
					(uint16) 63 ,
					(float) (viewport.X + (viewport.Width>>1)) , (float) (viewport.Y + (viewport.Height>>1)) ,
					64, 255, 64, 255 );
			// invulnerable
			if( Ships[WatchPlayerSelect.value].Invul )
				Print4x5Text( "Invulnerable" , FontWidth , render_info.window_size.cy-((FontHeight*4)+8) , 2 );
			// golden power pod
			if( Ships[WatchPlayerSelect.value].Object.Flags & SHIP_SuperNashram )
				Print4x5Text( "Golden Power Pod" , FontWidth , render_info.window_size.cy-((FontHeight*5)+10) , 2 );
			// stealthed
			if( Ships[WatchPlayerSelect.value].Object.Flags & SHIP_Stealth )
				Print4x5Text( "Stealth" , FontWidth , render_info.window_size.cy-((FontHeight*6)+12) , 2 );
	  }
    }
	else
	{
      if( Panel )
      {
        ScoreSort();
        PrintScoreSort();

		if(ShowMessages) 
			MessageQuePrintAll();
		else
			MessageQuePrint();

        if( DemoEyesSelect.value != MAX_PLAYERS )
          Print4x5Text( Names[DemoEyesSelect.value] ,render_info.window_size.cx - (FontWidth*9), FontHeight , 0 );
      }
    }
    if( Ships[WhoIAm].Object.Mode == GAMEOVER_MODE )
      CenterPrint4x5Text( "Game Over" , (render_info.window_size.cy >> 1) - (FontHeight*2) , 2 );

}

void ReleaseLevel(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_Title:
  case  STATUS_BetweenLevels:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    if( GodMode ) LoseAllWeapons();

    ReleaseComponentedShips();
    KillThermo();
    NodeRelease();
    CameraRelease();
    ReleaseExternalForces();
    ReleaseTeleports();
    ReleaseTriggers();
    ReleaseTriggerArea();
    ReleaseRTLights();
    ReleaseGoal();

    if( CountDownOn )
      DeleteCountdownDigits();

    KillAllBGObjects();
    ReleaseAllEnemies();
    ReleaseAllRestartPoints();
    DestroySound( DESTROYSOUND_All );
    break;
  }

}



void
ReleaseScene(void)
{
  int16 Count;

  if( RegenPoints != NULL )
  {
    free( RegenPoints );
    RegenPoints = NULL;
    NumRegenPoints = 0;

    for( Count = 0; Count < MAX_PLAYERS; Count++ )
    {
      if( RegenSlotsCopy[ Count ] != NULL )
      {
        free( RegenSlotsCopy[ Count ] );
        RegenSlotsCopy[ Count ] = NULL;
      }
    }
  }

  switch( MyGameStatus )
  {
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
    break;
  
  default:
    DestroyGame();
    break;
  }

}

void ReleaseView(void)
{
  switch( MyGameStatus )
  {
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_QuittingFromTitles:
  case  STATUS_WaitingToStartMultiPlayerClient:
  case  STATUS_WaitingToStartDemo:        // Added by DC 24/1/98

    ReleaseFlyGirl();

    FreeAllLastAFrameScrPolys();
    ReleaseTitle();
    ReleaseTloadheader( &Tloadheader );
    ReleaseTitleModels();
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
    break;

  case STATUS_ViewingScore:
#if 0 // used to release solid scr poly stuff
    ReleaseTloadheader( &Tloadheader );
    Free_All_Off_Files( &Title_OffsetFiles[ 0 ] );
    ReleaseRenderBufs();
#endif
    break;

  case STATUS_ViewingStats:
    break;

  default:

    Bspfree();

    
    FreeAllLastAFrameScrPolys();
    ReleaseMloadheader(&Mloadheader);
    ReleaseTloadheader( &Tloadheader );
    ReleaseModels();
    
    if ( MCloadheader.Buffer )
    {
      free( MCloadheader.Buffer );
      MCloadheader.Buffer = NULL;
    }
    if ( MCloadheadert0.Buffer )
    {
      free( MCloadheadert0.Buffer );
      MCloadheadert0.Buffer = NULL;
    }
	
	Free_All_Off_Files( &OffsetFiles[ 0 ] );
    ReleaseSkinExecs();
    ReleasePortalExecs();
    ReleaseRenderBufs();
  
    WaterRelease();
    
    FreeTxtFile();
    FreeMsgFile();
  }
}
//extern  uint16  BackgroundModel;

BOOL
InitScene(void)
{
  int i;
  //BackgroundModel = (uint16) -1;

  OnceOnlyInitModel();
  InitXLights();
  InitPrimBulls();
  InitSecBulls();
  InitFmPoly();

#ifdef SHADOWTEST
  InitTriangles();
#endif

  InitPolys();
  InitScrPolys();
  InitLines();
  SetupSpotFX();
  InitPickups();
  InitAmmoUsed();
  InitAfterBurners();
  InitBGObjects();
  InitRestartPoints();
  InitEnemies();

  for( i = 0 ; i < MAX_PLAYERS ; i++ )
  {
    Ships[i].ModelNum = (uint16) -1;
  }

  switch( MyGameStatus )
  {
  case  STATUS_BetweenLevels:
  case  STATUS_Title:
  case  STATUS_StartingSinglePlayer:
  case  STATUS_TitleLoadGameStartingSinglePlayer:
  case  STATUS_StartingMultiplayer:
  case  STATUS_GetPlayerNum:
    if (!LoadTitleModels())
    { Msg( "LoadTitleModels failed.\n" );
      return FALSE;
    }
    break;
  }
  
  return TRUE;

}

/*===================================================================
  Procedure :   Init the view...This might be title or score or main game....
  Output    :   BOOL TRUE/FLASE
===================================================================*/

BOOL InitView( void )
{
	DebugPrintf("InitView Starting...\n");

	CheatsDisabled = FALSE;

	reset_keyboard_buffer();

	InitModeCase();

	switch( MyGameStatus )
	{
		case  STATUS_BetweenLevels:
		case  STATUS_Title:
		case  STATUS_StartingSinglePlayer:
		case  STATUS_TitleLoadGameStartingSinglePlayer:
		case  STATUS_StartingMultiplayer:
		case  STATUS_GetPlayerNum:

		if( InitTitle() != TRUE ) // bjd
		{
			SeriousError = TRUE;
			return FALSE;
		}

		if ( !bSoundEnabled )
		{
			if (! InitializeSound( DESTROYSOUND_All ))
			{
				DebugPrintf("unable to initialise sound in initview\n");
				Msg("unable to initialise sound in initview\n");
				exit(1);
			}
		}

		InitRenderBufs();
	    
		if( !SetMatrixViewPort() )
		{
		  SeriousError = TRUE;
		  Msg( "SetMatrixViewPort() Failed\n" );
		  return FALSE;
		}
		// Init the Texture Handler
		InitTload( &Tloadheader );

		if( !Load_All_Off_Files( &Title_OffsetFiles[ 0 ] ) )
		{
		  SeriousError = TRUE;
		  return FALSE;
		}
	    
		if( !PreLoadFlyGirl() )
		{
		  SeriousError = TRUE;
		  return FALSE;
		}
	    

		if( !PreInitModel( TitleModelSet ) ) // bjd
		{
		  SeriousError = TRUE;
		  return FALSE;
		}
	    
		//  Load in And if nescessary ReScale Textures...
		if( !Tload( &Tloadheader ) )
		{
		  SeriousError = TRUE;
		  return FALSE;
		}

		if( !InitModel( TitleModelSet ) ) // bjd
		{
		  SeriousError = TRUE;
		  return FALSE;
		}
	      
		if ( !AllocateCompFlyGirl() )
		{
		  SeriousError = TRUE;
		  return FALSE;
		}
		else
		{
		  UpdateFlyGirl( &BikePos, &MATRIX_Identity, FALSE );
		}

		DummyTextureIndex = FindTexture( &Tloadheader, "data\\textures\\dummy.bmp" );
		if ( DummyTextureIndex != -1 )
		{
			if(!Tloadheader.PlaceHolderFile[ DummyTextureIndex ])
				Tloadheader.PlaceHolderFile[ DummyTextureIndex ] = ( char * )malloc( sizeof( char ) * 256 );
			Tloadheader.PlaceHolder[ DummyTextureIndex ] = TRUE;
		}

		if ( !CurrentMenu )
		  MenuRestart( &MENU_Start );

    break;

  case STATUS_ViewingScore:
    InitScoreDisplay();
    break;

  case STATUS_ViewingStats:
    InitStatsDisplay();
    break;
  
  default:
    
    // this will cause a lovely game loop and crash the game
	// so don't remove this !!!!!!!!!
    if( MyGameStatus != STATUS_InitView_0 )
		InitView_MyGameStatus = MyGameStatus;

    MyGameStatus = STATUS_InitView_0;
    FSClearBlack();
    CameraStatus = CAMERA_AtStart;  //prevents on screen menus from being suppressed
    break;
  }


  return TRUE;
}

/*===================================================================
  Procedure :   Set the View Matrix
  Input   :   nothing...  Current_Camera_View must be set
                  to a legal ship number...
  Output    :   nothing
===================================================================*/

void Build_View()
{
  VECTOR  Trans;
  VECTOR  Trans2;

  
  Trans.x = -CurrentCamera.Pos.x;
  Trans.y = -CurrentCamera.Pos.y;
  Trans.z = -CurrentCamera.Pos.z;

  ApplyMatrix( &CurrentCamera.InvMat, &Trans, &Trans2 );      /* Set Translation */

  view._11 = CurrentCamera.Mat._11; 
  view._12 = CurrentCamera.Mat._12;
  view._13 = CurrentCamera.Mat._13;
  view._14 = CurrentCamera.Mat._14;
             
  view._21 = CurrentCamera.Mat._21;
  view._22 = CurrentCamera.Mat._22;
  view._23 = CurrentCamera.Mat._23;
  view._24 = CurrentCamera.Mat._24;
             
  view._31 = CurrentCamera.Mat._31;
  view._32 = CurrentCamera.Mat._32;
  view._33 = CurrentCamera.Mat._33;
  view._34 = CurrentCamera.Mat._34;
                                        
  view._41 = Trans2.x;
  view._42 = Trans2.y;
  view._43 = Trans2.z;
  view._44 = CurrentCamera.Mat._44;
}


char TriggerName[256];

/*===================================================================
  Procedure :   Inits And Loads New Level..
  Input   :   nothing...
  Output    :   nothing
===================================================================*/
BOOL ChangeLevel( void )
{
	//if( NewLevelNum != LevelNum )
	//  return TRUE;

	LevelNum = NewLevelNum;

	//NumGoldBars = 0;

	ReleaseLevel();
	KillNodeCubeLines();
	KillBoxLines();

	// $$$
	NextworkOldBikeNum = -1;

	ActiveRemoteCamera = NULL;
	MissileCameraActive = 0;
	CameraMissile = (uint16) -1;

	InitScene();
	InitTextMessages();
	InitMultiSfxHandle();

	OnceOnlyChangeLevel = TRUE;
	ChangeLevel_MyGameStatus = MyGameStatus;
	Change_Ext( &LevelNames[ LevelNum ][ 0 ], &TriggerName[ 0 ], ".TRG" );
	if( !Triggerload( TriggerName ) )
	{
	  SeriousError = TRUE;
	  return( FALSE );
	}

	MyGameStatus = STATUS_ChangeLevelPostInitView;
	InitView();

	return( TRUE );
}

void GetLevelName( char *buf, int bufsize, int level )
{
  FILE *f;
  char *pCh;
  
  buf[ 0 ] = 0;

  if ( level < 0 )
    return;
  
  f = file_open( MissionTextNames[ level ], "r" );
  if ( f )
  {
    // read level name
    if ( !fgets( buf, bufsize, f ) )
    {
        // if error ( as oposed to end of file )
      if ( ferror( f ) != 0 )
      {
        buf[ 0 ] = 0;
      }
    }
  }
  if ( !buf[ 0 ] )
  {
    strncpy( buf, LevelList.item[ level ], bufsize );
  }

  pCh = strchr( buf, '\n' );
  if ( pCh )
    *pCh = 0;
  
  buf[ bufsize - 1 ] = 0; // ensure null terminated 
}

float Browl = 0.0F;
float HostMultiPlayerTimeout;

int colourflash = 0;
char NodeName[256];

/*===================================================================
  Procedure :   Game Status Control...
  Input   :   nothing...
  Output    :   nothing
===================================================================*/
/*===================================================================
  Procedure :   Game Status Control...
  Input   :   nothing...
  Output    :   nothing
===================================================================*/
extern void ReleaseView(void);
BOOL RenderScene( void )
{
  uint16  i,e;
  char  buf[256];
  //struct _stat stat_buf;
  //int result;
  static int WaitFrames = 2;
  BOOL done;

  //DebugPrintf("RenderScene Started\n");

  if ( SeriousError )
    return FALSE;

  // This is where in game we are getting input data read
  ReadInput();

  //if ( !Bsp_Identical( &Bsp_Header[ 0 ], &Bsp_Original ) )
  //  hr = 0;

  CalculateFramelag();
  AnimOncePerFrame++;

  if ( bSoundEnabled )
  {
    CheckSBufferList();
  }

  switch( MyGameStatus )
  {

  case STATUS_QuittingFromTitles:
	DebugState("STATUS_QuittingFromTitles\n");
    quitting = TRUE;
    break;

  case STATUS_WaitingToStartSinglePlayer:
	DebugState("STATUS_WaitingToStartSinglePlayer\n");

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    
    done = VduFinished( &MENU_NEW_BetweenLevels );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      StartASinglePlayerGame( NULL );
    }
    
    break;


  case STATUS_WaitingToStartDemo:
	DebugState("STATUS_WaitingToStartDemo\n");
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    break;


  case STATUS_WaitingToStartEndGameSequence:
	DebugState("STATUS_WaitingToStartEndGameSequence\n");
    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    done = VduFinished( &MENU_NEW_GeneralLoading );
      
    if ( done )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      MyGameStatus = STATUS_Title;
      ReleaseView();
    }
    break;


  case STATUS_WaitingToStartMultiPlayerHost:
	DebugState("STATUS_WaitingToStartMultiPlayerHost\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
		Browl = 30.0F;
		SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      MyGameStatus = STATUS_StartingMultiplayer;
      GoToSynchup( NULL );
    }
    break;


  case STATUS_WaitingToStartMultiPlayerClient:
	DebugState("STATUS_WaitingToStartMultiPlayerClient\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      MenuAbort();
      ReleaseView();
      LevelNum = -1;

      MyGameStatus = STATUS_PreStartingMultiplayerSynch;
      if( !ChangeLevel() ) 
        return( FALSE );
    }
    break;


  case STATUS_WaitingToStartTeamGame:
	DebugState("STATUS_WaitingToStartTeamGame\n");
    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }
    ReceiveGameMessages();

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    if ( VduFinished( NULL ) )
      WaitFrames--;
    if ( !WaitFrames )
    {
      WaitFrames = 2;
      LastMenu = CurrentMenu;
      VduClear();
      MenuAbort();
      MyGameStatus = PreSynchupStatus;
    }

    if( IMustQuit )
      SpecialDestroyGame();

    break;

  // the main menu screen
  case STATUS_Title:
	DebugState("STATUS_Title\n");

  // ??
  case STATUS_BetweenLevels:
	if(MyGameStatus != STATUS_Title)
		DebugState("STATUS_BetweenLevels\n");

    if( DisplayTitle() != TRUE )
    {
      SeriousError = TRUE;
      return FALSE;
    }
    break;


  case STATUS_Normal:
    DebugState("STATUS_Normal\n");

    if ( !CheatsDisabled )
      DisableCheats();
  
    if( IMustQuit )
      SpecialDestroyGame();

    if( MainGame() != TRUE ) // bjd
      return FALSE;

    if ( bSoundEnabled )
    {
      ProcessLoopingSfx();
#ifdef PLAYER_SPEECH_TAUNTS
      ProcessTaunt();
#endif
    }

    LevelTimeTaken += timer_run( &level_timer );

    // if player is quiting nothing should stop him.....
    if( MyGameStatus != STATUS_QuitCurrentGame )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
          for( i = 0 ; i < MAX_PLAYERS ; i++ )
            if( (GameStatus[i] != STATUS_LeftCrashed ) &&
				(GameStatus[i] != STATUS_Left        ) &&
				(GameStatus[i] != STATUS_Null        ) &&
                (GameStatus[i] != STATUS_Normal      ) &&
				i != WhoIAm )
                SendGameMessage(MSG_YOUQUIT, 0, (BYTE) i , 0, 0);

          // the level has ended or changed ..tell everyone to go onto new level
          MyGameStatus = STATUS_LevelEnd;
          GameStatus[WhoIAm] = MyGameStatus;
          RandomStartPosModify += 1;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
        }
      }
	  else
	  {
        if( OverallGameStatus == STATUS_LevelEnd )
        {
          // tell the host that I am now in the same state as him....
          MyGameStatus = OverallGameStatus;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        }
      }
    }

    if ( WaitingToQuit )
    {
      FSClearBlack();
      CenterPrint4x5Text( PLEASE_WAIT, (render_info.window_size.cy>>1)-(FontHeight>>1) , 2 );
      SelectQuitCurrentGame( NULL );
    }
    break;


  case STATUS_LevelEnd:
	DebugState("STATUS_LevelEnd\n");
    FSClearBlack();
    ReceiveGameMessages();
    Browl -= framelag;

    StopDemoRecording();

    if( Browl <= 0.0F )
    {
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	  else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }
    if( IsHost )
    {
      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( Ships[i].enable != 0 ) && ( GameStatus[i] != STATUS_LevelEnd ) && ( i != WhoIAm ) )
          break;
      }
      if( i != MAX_PLAYERS )
        break;  // not yet..

      // Added the if check to not call OutputVisiStats improperly
      // The order of code will always result in LevelNum being -1 here
      // So no point in even leaving this code uncommented...
      // We'll probably just end up getting rid of visi stats anyway later
      //if ( LevelNum != -1 )
      //	OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );

      // un load all graphics
      ReleaseView();
      ReleaseLevel();
	  
      // tell them all they can now look at the scores...
      MyGameStatus = STATUS_ViewingScore;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);

      InitScene();  // STATSTEST
      InitView();
	  // $$$
      NextworkOldBikeNum = -1;
      HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;

    }
	else
	{
      if( MyGameStatus != OverallGameStatus )
      {

        OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
        // un load all graphics
        ReleaseView();
        ReleaseLevel();
        // tell the host that I am now in the same state as him....
        MyGameStatus = STATUS_ViewingScore;

        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

        InitScene();  // STATSTEST
        InitView();
		// $$$
        NextworkOldBikeNum = -1;
        HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F * 2.0F;
      }
    }
    break;


  case STATUS_ViewingScore:
	DebugState("STATUS_ViewingScore\n");

    ReceiveGameMessages();

    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }

    ScoreDisplay();

    HostMultiPlayerTimeout -= framelag;

    if( buffered_key_released( SDLK_SPACE ) || ( OverallGameStatus == STATUS_WaitingAfterScore ) )
    {
		FSClearBlack();
		HostMultiPlayerTimeout = 60.0F * 60.0F * 2.0F;

      if( IsHost )
      {
          // tell them all I Am waiting
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
          if( !ChangeLevel() )
			  return( FALSE );
      }
	  else
	  {
          // tell the host that I am now Waiting for him to finish viewing the score....
          ReleaseView();
          MyGameStatus = STATUS_WaitingAfterScore;
          GameStatus[WhoIAm] = MyGameStatus;
          if( !ChangeLevel() )
			  return( FALSE );
      }
    }
    break;


  case STATUS_WaitingAfterScore:
	DebugState("STATUS_WaitingAfterScore\n");

    InitFontTransTable( TRUE );
    FSClearBlack();
    e = 0;

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;
    }

		// you must clear player scores if MaxKills limit is set...
		// other wise the game just keeps on changing levels...
		// cause at the start of each level they already have max kills...

		// reset all player's stats
		if( MaxKills || ResetKillsPerLevel )
			ResetAllStats();

      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {      
        if( (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {

		//        if( GameStatus[i] == STATUS_ViewingScore )
		//        {
		//          sprintf( buf, "%-8s status %8s\n", &Names[i][0] , "viewing score" );
		//          CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
		//        }
		//        else
          {
            sprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
            CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_WaitingAfterScore ? 2 : 1 );
          }
          
          e++;
        }
      }

    CenterPrint4x5Text( "waiting for all other players" , (render_info.window_size.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);

    Browl -= framelag;
    if( Browl < 0.0F )
    {
      colourflash++;
      if( IsHost )
          SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
      else
          SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      Browl = 30.0F;
    }
    ReceiveGameMessages();

    if( IsHost )
    {

      // have all the enabled players switched over to STATUS_LevelEnd??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( ( ( GameStatus[ i ] >= STATUS_InitView_0 ) && ( GameStatus[ i ] <= STATUS_InitView_9 ) ) || ( GameStatus[i] == STATUS_ViewingScore ) ) && ( i != WhoIAm ) ) break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
      }

      //DebugPrintf("STATUS_WaitingAfterScore setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      InitFontTransTable();
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);

    }else{
      if( OverallGameStatus == STATUS_Normal )
      {
        InitFontTransTable();

        if( CountDownOn )
        {
          CreateCountdownDigits();
          StartCountDown( (int16) TimeLimit.value, 0 );
        }
        // tell the host that I am now in the same state as him....
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      }
    }
    break;


  case STATUS_Joining:
	DebugState("STATUS_Joining\n");
    FSClearBlack();

    ReceiveGameMessages();
    if( IMustQuit || IsHost || ( OverallGameStatus != STATUS_Normal ) )
    {
      SpecialDestroyGame();
      break;
    }
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    CenterPrint4x5Text( "Pickups Left   " , (render_info.window_size.cy>>1)-(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Pickups , (render_info.window_size.cx>>1)+((17*FontWidth>>1)), (render_info.window_size.cy>>1)-(FontHeight<<2), 2 );

    CenterPrint4x5Text( "RegenSlots Left" , (render_info.window_size.cy>>1)-(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].RegenSlots , (render_info.window_size.cx>>1)+((17*FontWidth>>1)), (render_info.window_size.cy>>1)-(FontHeight<<1), 2 );

    CenterPrint4x5Text( "Mines Left     " , (render_info.window_size.cy>>1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Mines , (render_info.window_size.cx>>1)+((17*FontWidth>>1)), (render_info.window_size.cy>>1), 2 );

    CenterPrint4x5Text( "Triggers Left  " , (render_info.window_size.cy>>1)+(FontHeight<<1), 8 );
    Printuint16( (uint16) Ships[WhoIAm].Triggers , (render_info.window_size.cx>>1)+((17*FontWidth>>1)), (render_info.window_size.cy>>1)+(FontHeight<<1), 2 );

    CenterPrint4x5Text( "TrigVars Left  " , (render_info.window_size.cy>>1)+(FontHeight<<2), 8 );
    Printuint16( (uint16) Ships[WhoIAm].TrigVars , (render_info.window_size.cx>>1)+((17*FontWidth>>1)), (render_info.window_size.cy>>1)+(FontHeight<<2), 2 );
    
    // wait for all the pickup and mine stuff to be sent to me.....
    if( ( Ships[WhoIAm].Pickups == 0 ) && ( Ships[WhoIAm].Mines == 0 ) && ( Ships[WhoIAm].RegenSlots == 0 ) &&
      ( Ships[WhoIAm].Triggers == 0 ) && ( Ships[WhoIAm].TrigVars == 0 ) && OverallGameStatus == STATUS_Normal)
    {
      // broadcast "x is joining the game" to all players...
      if (TeamGame)
        SendGameMessage(MSG_TEXTMSG, 0, 0, TEXTMSGTYPE_JoiningTeamGame, 0);

      MyGameStatus = OverallGameStatus;
	  
#ifdef DEMO_SUPPORT
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
#endif

      GameStatus[WhoIAm] = OverallGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
      SyncMines();
      InitShipRandomPos( WhoIAm );
      NextworkOldBikeNum = -1;
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) TimeLimit.value, 0 );
        IllegalTime = TRUE;
      }
    }
    break;

  // in the multiplayer screen waiting for the game to start
  case STATUS_StartingMultiplayer:
	DebugState("STATUS_StartingMultiplayer\n");

    Browl -= framelag;

    if( Browl <= 0.0F )
    {
      Browl = 30.0F;
      if( TeamGame &&  (OverallGameStatus == STATUS_Normal) )
	  {
		  //SendGameMessage(MSG_SENDKILLSDEATHSBIKENUM, 0, 0, 0, 0);
	  }
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    ReceiveGameMessages();

    if( ( MyGameStatus == STATUS_QuitCurrentGame ) || ( MyGameStatus == STATUS_Title ) )
    {
      break;
    }
    
    if( ( CurrentMenu ) && ( CurrentMenuItem ) )
    {
      if( DisplayTitle() != TRUE )
        return FALSE;
    }

    // if status has been changed from titles, should exit this state straight away
    if ( MyGameStatus != STATUS_StartingMultiplayer )
      break;

    if( ( IsHost ) && ( !CurrentMenu ) && ( !CurrentMenuItem ) )
    {
		FSClearBlack();
      ReleaseView();
      // tell them all to load up a level
      MyGameStatus = STATUS_StartingMultiplayerSynch;
      GameStatus[WhoIAm] = MyGameStatus;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

      LevelNum = -1;
      if( !ChangeLevel() ) return( FALSE );
      HostMultiPlayerTimeout = 60.0F * 15.0F;
    }
    else
    {
      if( !IsHost )
      {
        if( ( OverallGameStatus == STATUS_StartingMultiplayerSynch ) || 
          ( (OverallGameStatus == STATUS_Normal) && (!TeamGame || PlayerReady[WhoIAm]) ) )
        {

          if ((CameraStatus == CAMERA_AtLeftVDU) || (CameraStatus == CAMERA_AtRightVDU))
          { 
            //LastMenu = CurrentMenu;
            //VduClear();
            MenuChangeEx( &MENU_NEW_GeneralLoading );
            MenuDraw( CurrentMenu );  // because menu processing will not be done when MyGameStatus changes
          }
		  else
		  {
				FSClearBlack();
		  }


          PreSynchupStatus = MyGameStatus;
          MyGameStatus = STATUS_WaitingToStartMultiPlayerClient;
        }
      }
    }
    break;


  case  STATUS_PreStartingMultiplayerSynch:
	DebugState("STATUS_PreStartingMultiplayerSynch\n");

    ReceiveGameMessages();
    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }
    MyGameStatus = STATUS_StartingMultiplayerSynch;
    if( OverallGameStatus == STATUS_Normal )
    {
      initShip( WhoIAm );
      MyGameStatus = STATUS_Joining;
      Browl = 0.0F;
    }else{
      smallinitShip( WhoIAm );
      MyGameStatus = STATUS_StartingMultiplayerSynch;
    }
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    GameStatus[WhoIAm] = MyGameStatus;
    break;


  case  STATUS_StartingMultiplayerSynch:
	DebugState("STATUS_StartingMultiplayerSynch\n");

    InitFontTransTable( TRUE );

    ReceiveGameMessages();

    if( IMustQuit )
    {
      SpecialDestroyGame();
      break;
    }

    FSClearBlack();
    e = 0;

      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( ( GameStatus[i] != STATUS_GetPlayerNum )&& (GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && (GameStatus[i] != STATUS_Null ) )
        {
          sprintf( buf, "%-8s status %14s\n", &Names[i][0] , &StatusTab[ GameStatus[i] ][0] );
          CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy>>1)-( ( (FontHeight+2) * MAX_PLAYERS ) >> 1 )+ (e * (FontHeight+(FontHeight>>1)) ) , GameStatus[i] == STATUS_StartingMultiplayerSynch ? 2 : 1 );
          e++;
        }
      }

    CenterPrint4x5Text( "all players synching" , (render_info.window_size.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , (colourflash>>3) &3);
    Browl -= framelag;
    if( Browl <= 0.0F )
    {
      colourflash++;
      Browl = 30.0F;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    if( CountDownOn )
    {
      CreateCountdownDigits();
      StartCountDown( (int16) TimeLimit.value, 0 );
      IllegalTime = FALSE;
    }

    if( IsHost )
    {
      Printuint16( (uint16) (HostMultiPlayerTimeout / 60.0F) , 0 , 0 , 2 );
      HostMultiPlayerTimeout -= framelag;

      // have all the enabled players switched over to STATUS_StartingMultiplayer??
      for( i = 0 ; i < MAX_PLAYERS ; i++ )
      {
        if( (GameStatus[i] != STATUS_GetPlayerNum ) &&(GameStatus[i] != STATUS_LeftCrashed ) && (GameStatus[i] != STATUS_Left ) && ( (GameStatus[i] != STATUS_Null ) && ( GameStatus[i] != STATUS_StartingMultiplayerSynch ) ) && ( i != WhoIAm ) )
          break;
      }
      if( HostMultiPlayerTimeout >= 0.0F )
      {
        if( i != MAX_PLAYERS )
          break;  // not yet..
      }
      // tell them all they can now restart a new level...
#ifdef DEMO_SUPPORT
      QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
#endif
      DebugPrintf("STATUS_StartingMultiplayerSynch setting MyGameStatus to STATUS_Normal\n");
      MyGameStatus = STATUS_Normal;
      GameStatus[WhoIAm] = STATUS_Normal;
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

	  // $$$
      NextworkOldBikeNum = -1;
      InitFontTransTable();

    }
	else
	{
      if( OverallGameStatus == STATUS_Normal )
      {
#ifdef DEMO_SUPPORT
        QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
#endif
        MyGameStatus = OverallGameStatus;
        GameStatus[WhoIAm] = MyGameStatus;
        SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
        // $$$
        NextworkOldBikeNum = -1;
        InitFontTransTable();
      }
    }
    break;
  

  case STATUS_QuitCurrentGame:
	DebugState("STATUS_QuitCurrentGame\n");

    OutputVisiStats( &Mloadheader, LevelNames[ LevelNum ] );
    MenuAbort();
    MenuRestart( &MENU_Start );
    DestroyGame();  // DestroyGame changes MyGameStatus..So Set it back
    MyGameStatus = STATUS_QuitCurrentGame;
    ReleaseView();
    ReleaseLevel();
    MyGameStatus = STATUS_Title;
    InitScene();
    InitView();

    break;

  case STATUS_GetPlayerNum:
	DebugState("STATUS_GetPlayerNum\n");

    FSClearBlack();
    ReceiveGameMessages();
    
    CenterPrint4x5Text( "Requesting Player Number" , (render_info.window_size.cy>>1)-(FontHeight>>1) + ( ( FontHeight+2) * (MAX_PLAYERS+1)) , 2 );
                                          
    sprintf( &buf[0] , "Attempt %d\n" , GetPlayerNumCount );
    CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy>>1)-(FontHeight>>1) , 2 );

    GetPlayerNumCount1 -= framelag;
    GetPlayerNumCount2 -= framelag;

    if( WhoIAm != UNASSIGNED_SHIP )
	{
		if(WhoIAm >= MAX_PLAYERS)
		{
			switch(WhoIAm)
			{
			case MAX_PLAYERS:
				PrintErrorMessage ( "The Game is Currently Un-Joinable", 3, NULL, ERROR_USE_MENUFUNCS );
				MyGameStatus = STATUS_Title;
				break;
			case MAX_PLAYERS+1:
				PrintErrorMessage ( "You Dont have the level they are playing", 3, NULL, ERROR_USE_MENUFUNCS );
				MyGameStatus = STATUS_Title;
				break;
			case MAX_PLAYERS+2:
				PrintErrorMessage ( "The Game is Full", 3, NULL, ERROR_USE_MENUFUNCS );
				MyGameStatus = STATUS_Title;
				break;
			case MAX_PLAYERS+3:
				PrintErrorMessage ( "You do have the right version", 3, NULL, ERROR_USE_MENUFUNCS );
				MyGameStatus = STATUS_Title;
				break;
			default:
				PrintErrorMessage ( "Could not join game host gave no reason why", 3, NULL, ERROR_USE_MENUFUNCS );
				MyGameStatus = STATUS_Title;
			}
			break; // we are done
		}

		// go to next menu
		MenuState = MENUSTATE_Select;
		MenuChangeEx( GetPlayerNumMenu );

		break;
	}

    if( GetPlayerNumCount2 <= 0.0F )
    {
      // Time is Up No Player Number was got so quit back to the title screen...
      PrintErrorMessage ( "Couldnt Get a Player Number", 3, NULL, ERROR_USE_MENUFUNCS );
      MyGameStatus = STATUS_Title;
      break;
    }

    if( GetPlayerNumCount1 <= 0.0F )
    {
      GetPlayerNumCount++;

      // Time to send another request for a Player Number...
      GetPlayerNumCount1 = 60.0F;   // approx 1 Second...
      SendGameMessage(MSG_HEREIAM, 0, 0, 0, 0); // can I join your game...
      DebugPrintf("sending MSG_HEREIAM\n");
    }

    break;

#ifdef DEMO_SUPPORT
  case STATUS_PlayingDemo:
	DebugState("STATUS_PlayingDemo\n");

    DemoGameLoops++;

    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
    
    Oldframelag = framelag;

    if( PauseDemo )
    {
      framelag = 0.0F;
    }else{
      framelag *= Demoframelag;
    }

    if( MainGame() != TRUE ) // bjd
      return FALSE;

    break;
#endif

  case STATUS_InitView_0:
	DebugState("STATUS_InitView_0\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();

    MenuFrozen = FALSE; // ensure that menus are OK to use once in game
    JustExitedMenu = FALSE; 

    QuickStart = QUICKSTART_None;
    WaitingToQuit = FALSE;
  
    ReceiveGameMessages();
  
    if( !SetMatrixViewPort() )
    {
      SeriousError = TRUE;
      Msg( "SetMatrixViewPort() failed\n" );
      return FALSE;
    }
    
    InitFont();

    MyGameStatus = STATUS_InitView_1;
    //PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_1:
	DebugState("STATUS_InitView_1\n");
    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();

    ReceiveGameMessages();

    ReMakeSimplePanel = TRUE;
    
	//    InitVisiExecList( lpDev );
    InitSkinExecs();
    InitPortalExecs();
    InitRenderBufs(/* lpDev */); // bjd

    // Init the Texture Handler
    if( !InitTload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "InitTLoad failed\n" );
      return FALSE;
    }
  
    //  Prep the Texture Handler.....
    if( !PreMload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }


    // Can Cope with no .Wat file!!!
    PreWaterLoad( (char*) &WaterNames[LevelNum][0] );
    
    
    if( OnceOnlyChangeLevel )
    {
      OnceOnlyChangeLevel = FALSE;

      if( !PreLoadShips() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadBGOFiles() )
      {
        SeriousError = TRUE;
        return FALSE;
      }

      if( !PreLoadRestartPoints() )
      {
        SeriousError = TRUE;
        return FALSE;
      }
      if( !PreLoadEnemies() )
      {
        SeriousError = TRUE;
        return FALSE;
      }
    }

    EnableRelavantModels( &ModelNames[0] );

	if( !PreInitModel( /*lpDev,*/ &ModelNames[0] ) ) // bjd
	{
		SeriousError = TRUE;
		return FALSE;
	}

	if( !Load_All_Off_Files( &OffsetFiles[ 0 ] ) )
	{
		SeriousError = TRUE;
		return FALSE;
	}

    MyGameStatus = STATUS_InitView_2;
    //PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_2:
	DebugState("STATUS_InitView_2\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();

    ReceiveGameMessages();

    //  Load in And if nescessary ReScale Textures... 
    if( !Tload( &Tloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE;
    }

    MyGameStatus = STATUS_InitView_3;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_3:
	DebugState("STATUS_InitView_3\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();

    ReceiveGameMessages();

      if( !InitModel( /*lpDev,*/ &ModelNames[0] ) ) // bjd
        {
        SeriousError = TRUE;
        return FALSE;               // all 3d models....
      }
    MyGameStatus = STATUS_InitView_4;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_4:
	DebugState("STATUS_InitView_4\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();

    ReceiveGameMessages();

    if( !Mload( (char*) &LevelNames[LevelNum][0] , &Mloadheader ) )
    {
      SeriousError = TRUE;
      return FALSE; // the model and visipoly data
    }

    InitVisiStats( &Mloadheader );

    MyGameStatus = STATUS_InitView_5;
    PrintInitViewStatus( MyGameStatus );
    break;
  

  case STATUS_InitView_5:
	DebugState("STATUS_InitView_5\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();
    ReceiveGameMessages();

    // Can Cope with no Bsp file!!!
#ifdef LOAD_ZBSP
    Bspload( (char*) &BspZNames[LevelNum][0], &Bsp_Header[ 0 ] );
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 1 ] );
#else
#ifdef BSP_ONLY
    if ( !Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ) )
    {
      SeriousError = TRUE;
      Msg( "Bspload failed\n" );
      return FALSE;   // the collision data
    }
#else
    Bspload( (char*) &BspNames[LevelNum][0], &Bsp_Header[ 0 ] ); // load .BSP file into 0 skin
#endif
    Bsp_Header[ 1 ].State = FALSE; // no non-zero .BSP any more
#endif
	//    if ( !Bsp_Duplicate( &Bsp_Header[ 0 ], &Bsp_Original ) )
	//    {
	//		    SeriousError = TRUE;
	//		    Msg( "Bsp_Duplicate failed\n" );
	//		    return FALSE;
	//    }
    
    // might not be any water...
    WaterLoad();

    ReadTxtFile( (char*) &TextNames[LevelNum][0] );
    
    ReadMsgFile( (char*) &MsgNames[LevelNum][0] );
    
    if( !MCload( (char*) &CollisionNames[LevelNum][0] , &MCloadheader ) )
    {
      SeriousError = TRUE;
      Msg( "MCload non zero failed\n" );
      return FALSE;   // the collision data
    }
    if( !MCload( (char*) &CollisionZNames[LevelNum][0] , &MCloadheadert0 ) )
    {
      SeriousError = TRUE;
      Msg( "MCload zero failed\n" );
      return FALSE; // the collision data skin thickness 0
    }
  
    SetUpShips();

    MyGameStatus = STATUS_InitView_6;
    PrintInitViewStatus( MyGameStatus );

    break;

  case STATUS_InitView_6:
	DebugState("STATUS_InitView_6\n");

    if( IsHost )
    {
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
    }
	else
	{
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    }

    FSClearBlack();
    ReceiveGameMessages();

    InitSoundInfo( &Mloadheader );
    
    MyGameStatus = STATUS_InitView_7;
    PrintInitViewStatus( MyGameStatus );
    break;


  case STATUS_InitView_7:
	DebugState("STATUS_InitView_7\n");

    if( IsHost )
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	else
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

    FSClearBlack();
    ReceiveGameMessages();
#ifdef NO_PRECALCULATED_CELL_COLOURS
    CreateCellColours( &Mloadheader );
#endif
    MyGameStatus = STATUS_InitView_8;
    PrintInitViewStatus( MyGameStatus );
    break;


  case STATUS_InitView_8:
	DebugState("STATUS_InitView_8\n");

    if( IsHost )
      SendGameMessage(MSG_LONGSTATUS, 0, 0, 0, 0);
	else
      SendGameMessage(MSG_STATUS, 0, 0, 0, 0);

    FSClearBlack();
    ReceiveGameMessages();
	DebugState("STATUS_InitView_9\n");
    MyGameStatus = STATUS_InitView_9;
    PrintInitViewStatus( MyGameStatus );

    InitShipSpeeds();

    // this will cause a lovely game loop and crash the game
	// so don't remove this !!!!!!!!!
    if( InitView_MyGameStatus != STATUS_InitView_0 )
		MyGameStatus = InitView_MyGameStatus;

	//
	input_grab( TRUE );

    break;


  case STATUS_ChangeLevelPostInitView:
	DebugState("STATUS_ChangeLevelPostInitView\n");

    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".NOD" );
    if( !Nodeload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadBGOFiles() )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    if( !LoadEnemies() )
    {
      SeriousError = TRUE;
      Msg( "LoadEnemies() failed\n" );
      return( FALSE );
    }

    if( !LoadSpotFX() )
    {
      SeriousError = TRUE;
      Msg( "LoadSpotFX() failed\n" );
      return( FALSE );
    }

    if( !LoadStartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadStartPoints() failed\n" );
      return( FALSE );
    }

    if( !LoadRestartPoints() )
    {
      SeriousError = TRUE;
      Msg( "LoadRestartPoints() failed\n" );
      return( FALSE );
    }

      if ( !InitializeSound( DESTROYSOUND_All ))
      {
        Msg("InitializeSound() failed\n");
        return FALSE;
      }


    Change_Ext( &LevelNames[ LevelNum ][ 0 ], &NodeName[ 0 ], ".CAM" );

    if( !Cameraload( NodeName ) )
    {
      SeriousError = TRUE;
      return( FALSE );
    }

    // might not be any real-time lights
    LoadRTLights();

    if ( CaptureTheFlag || CTF )
      GoalLoad();

    InitThermo();

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      LoadTimeForLevel();
    }

    if( !LoadPickupsPositions() )
    {
      SeriousError = TRUE;
      Msg( "LoadPickupsPositions() failed\n" );
      return( FALSE );
    }

    // might not be any External Forces...
    ExternalForcesLoad( (char*) &ExternalForceNames[LevelNum][0] );

    // might not be any Teleports...
    TeleportsLoad( (char*) &TeleportNames[LevelNum][0] );
	
    // Can Cope with no Zone file!!!
    TriggerAreaload( (char*) &ZoneNames[LevelNum][0] );


    InitShipsChangeLevel(&Mloadheader);

    if( ( ChangeLevel_MyGameStatus == STATUS_SinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_PostStartingSinglePlayer ) ||
      ( ChangeLevel_MyGameStatus == STATUS_TitleLoadGamePostStartingSinglePlayer) )
    {
      if( CountDownOn )
      {
        CreateCountdownDigits();
        StartCountDown( (int16) SinglePlayerTimeLimit, 0 );
      }
      else
      {
        DeleteCountdownDigits();
      }
    }

    GodModeOnceOnly = TRUE;

    MyGameStatus = ChangeLevel_MyGameStatus;

    break;


#ifdef DEMO_SUPPORT
  case STATUS_ChangeLevelPostPlayingDemo:
	DebugState("STATUS_ChangeLevelPostPlayingDemo\n");

    WhoIAm = MAX_PLAYERS;
    Ships[WhoIAm].Object.Mode = DEMO_MODE;
    Ships[WhoIAm].enable = 1;
    IsHost = TRUE;
    Current_Camera_View = MAX_PLAYERS;    // which object is currently using the camera view....
  
    for( i = 0 ; i < MAX_PLAYERS ; i++ )
    {
      GameStatus[i] = STATUS_Null;
    }
  
    DemoTimeSoFar = 0;
    if( DemoSpeed.value > 8 )
    {
      // slower or normal playback speed...
      Demoframelag = 1.0F / (float) ( DemoSpeed.value - 7 );
    }else{
      Demoframelag = 1.0F * (float) ( 9 - DemoSpeed.value );
    }
  
    GameElapsedTime = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
    QueryPerformanceCounter((LARGE_INTEGER *) &DemoStartedTime);
    DemoGameLoops = 0;
    TempGameElapsedTime = GameStartedTime;
    MyGameStatus = STATUS_PlayingDemo;
    break;
#endif

	//  *********************** Single Player Game Stuff **********************************


  case  STATUS_StartingSinglePlayer:
	DebugState("STATUS_StartingSinglePlayer\n");

    //FSClearBlack();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_PostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    SendGameMessage(MSG_STATUS, 0, 0, 0, 0);
    LevelNum = -1;
    if( !NewLevelNum )
    {
      NumGoldBars = 0;
      CrystalsFound = 0;
    }
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_PostStartingSinglePlayer:
	DebugState("STATUS_PostStartingSinglePlayer\n");

    smallinitShip( WhoIAm );
	//    if( CountDownOn )
	//    {
	//      CreateCountdownDigits();
	//      StartCountDown( (int16) TimeLimit.value, 0 );
	//    }
    
#ifdef DEMO_SUPPORT
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
#endif

    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelTimeTaken = 0.0F;
    timer_run( &level_time );

    break;


  case STATUS_SinglePlayer:
	DebugState("STATUS_SinglePlayer\n");

    if( MainGame() != TRUE ) // bjd
      return FALSE;

    if ( bSoundEnabled && !CurrentMenu )
    {
      ProcessLoopingSfx();
      ProcessEnemyBikerTaunt();
    }

    LevelTimeTaken += timer_run( &level_timer );

    //sprintf( buf, "time so far %d", ((uint32)LevelTimeTaken) / 100 );
    //Print4x5Text( buf, 10, 30, 2 );

    // if player is quiting nothing should stop him.....
    if( MyGameStatus != STATUS_QuitCurrentGame )
    {
      if( IsHost )
      {
        if( NewLevelNum != LevelNum )
        {
			FSClearBlack();
          ReleaseView();
          // the level has ended or changed...
          MyGameStatus = STATUS_ViewingStats;
          GameStatus[WhoIAm] = MyGameStatus;
          InitView();
        }
      }
    }
    break;


  case STATUS_ViewingStats:
	DebugState("STATUS_ViewingStats\n");

	FSClearBlack();

    ReleaseLevel();

    ReleaseView();
      
    MyGameStatus = STATUS_BetweenLevels;
    InitScene();
    InitView();
    MenuAbort();
    MenuRestart( &MENU_NEW_NumberOfCrystals );
    CameraStatus = CAMERA_AtRightVDU;
        break;


  case  STATUS_TitleLoadGameStartingSinglePlayer:
	DebugState("STATUS_TitleLoadGameStartingSinglePlayer\n");

    //FSClearBlack();
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_InGameLoadGameStartingSinglePlayer:
	DebugState("STATUS_InGameLoadGameStartingSinglePlayer\n");

    //FSClearBlack();

    NewLevelNum = InGameLoadGameLevelNum;
    MenuAbort();
    ReleaseView();
    // tell them all to load up a level
    MyGameStatus = STATUS_TitleLoadGamePostStartingSinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    LevelNum = -1;
    NumGoldBars = 0;
    CrystalsFound = 0;
    GameCompleted = GAMECOMPLETE_NotComplete;
    if( !ChangeLevel() ) return( FALSE );
    break;


  case  STATUS_TitleLoadGamePostStartingSinglePlayer:
	DebugState("STATUS_TitleLoadGamePostStartingSinglePlayer\n");

    smallinitShip( WhoIAm );
    InGameLoad( NULL );
    
#ifdef DEMO_SUPPORT
    QueryPerformanceCounter((LARGE_INTEGER *) &GameStartedTime);
#endif

    MyGameStatus = STATUS_SinglePlayer;
    GameStatus[WhoIAm] = MyGameStatus;
    break;

  default:
	DebugPrintf("MyGameStatus has a bad setting!!!!\n");
	break;

	//  ******************** End of Single Player Game Stuff *******************************

  }

 // DebugPrintf("RenderScene Ended\n");

  return TRUE;
}


float Hdiv[MAX_PLAYERS] = { 1.0F ,
                1.0F ,
                1.0F ,
                0.5F ,
                0.5F ,
                0.5F ,
                1.0F ,
                0.5F };
float Vdiv[MAX_PLAYERS] = { 1.0F ,
                0.5F ,
                0.33333333F ,
                0.5F ,
                0.2F ,
                0.33333333F ,
                0.142857142F ,
                0.25F  };
float Hpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 0.0F } ,
                       { 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F } ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F } ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F} ,
                       { 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F , 0.0F} ,
                       { 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F , 0.0F , 1.0F } };
float Vpos[MAX_PLAYERS][MAX_PLAYERS] = { { 0.0F } ,
                       { 0.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F } ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F } ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F} ,
                       { 0.0F , 1.0F , 2.0F , 3.0F , 4.0F , 5.0F , 6.0F} ,
                       { 0.0F , 0.0F , 1.0F , 1.0F , 2.0F , 2.0F , 3.0F , 3.0F } };
                       

/*===================================================================
  Procedure :   Main Routines...
  Input   :   nothing...
  Output    :   nothing
===================================================================*/
void MainRoutines( void )
{
  if( PlayDemo )
  {
    DemoPlayingNetworkGameUpdate();
  }

  ProcessShips();

#ifdef SHADOWTEST
//  CreateSpotLight( (uint16) WhoIAm, SHIP_RADIUS, &Mloadheader );
//  CreateShadowsForShips();
#endif

  FirePrimary();
  FireSecondary();
  ProcessEnemies();
  ProcessSpotFX();
  ProcessPrimaryBullets();
  ProcessSecondaryBullets();
  if( !PlayDemo ) RegeneratePickups();
  ProcessPickups();
  ProcessBGObjects( TRUE );
  ProcessRestartPoints();
  ProcessModels();
  ProcessPolys();
  ProcessXLights( &Mloadheader );
  DoAfterBurnerEffects();
  FmPolyProcess();
  CheckTimeLimit();
  if( CountDownOn ) UpdateCountdownDigits();
  ShowScreenMultiples();
  ProcessActiveConditions();
  ProcessTriggerAreas();
  ProcessGoals();
#ifdef DOESNT_WORK_AND_A_BAD_IDEA_ANYWAY
  if ( outside_map && !DebugInfo && ( Ships[WhoIAm].Object.Mode != DEATH_MODE ) && ( Ships[WhoIAm].Object.Mode != LIMBO_MODE ) )
  {
    // bugger -- the player has managed to get outside the map, so kill him off...
    Ships[WhoIAm].ShipThatLastHitMe = MAX_PLAYERS;
    Ships[WhoIAm].Damage = 255 + 255; //make sure I Die....
    DoDamage( OVERRIDE_INVUL );
  
  
    Ships[WhoIAm].Object.Mode = DEATH_MODE;
    Ships[WhoIAm].Timer = 0.0F;
    ShipDiedSend( WEPTYPE_Primary, 0);
  }
#endif
  WaterProcess();
  ProcessRTLights();
  
	//DebugPrintf("MainRoutines Finished...\n");
}

void CheckLevelEnd ( void )
{

  if( HasLevelEnded() )
  {
    PauseAllSfx();
#ifdef FINAL_RELEASE
    if( OriginalLevels )
    {
#endif
      LevelNum = -1;
      if( NewLevelNum == ( LevelList.items - 2 ) )
      {
        // if they cheated at any point
        if ( Cheated )
        {
          GameCompleted = GAMECOMPLETE_Cheated;
        }else
        {
          // Check for Secret level...
          if( CrystalsFound == ( LevelList.items - 1 ) )
          {
            //They can go to the secret level
            NewLevelNum = ( LevelList.items - 1 );
            GameCompleted = GAMECOMPLETE_GotAllCrystalsSoFar;
          }else{
            // go straight to the end sequence
            GameCompleted = GAMECOMPLETE_WithoutAllCrystals;
          }
        }
      }else if( NewLevelNum == ( LevelList.items - 1 ) ){
        // They have completed the Secret level
        if( CrystalsFound == ( LevelList.items ) )
        {
          GameCompleted = GAMECOMPLETE_WithAllCrystals;
        }else
        {
          GameCompleted = GAMECOMPLETE_WithAllCrystalsExceptLast;
        }
      }else{
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
      
      return;
#ifdef FINAL_RELEASE
    }else
    {
      if( NewLevelNum == ( LevelList.items - 1 ) )
      {
        // completed all levels ( not original levels )
        GameCompleted = GAMECOMPLETE_AdditionalLevels;
        return;
      }else
      {
        NewLevelNum++;
        GameCompleted = GAMECOMPLETE_NotComplete;
      }
    }
#endif
  }

}

/*===================================================================
  Procedure :   Main Render Loop...
  Input   :   nothing...
  Output    :   nothing
===================================================================*/

BOOL MainGame( void ) // bjd
{
  int i;
  static float fov_inc = 0.0F;

  // For stereo
  VECTOR cam_offset;

#ifdef DEMO_SUPPORT
  QueryPerformanceCounter((LARGE_INTEGER *) &GameCurrentTime);
  if( PlayDemo )
  {
    if( PauseDemo )
    {
      TempGameElapsedTime = GameCurrentTime;
    }else{
      GameElapsedTime += (LONGLONG) ( ( GameCurrentTime - TempGameElapsedTime ) * Demoframelag );
      TempGameElapsedTime = GameCurrentTime;
      GameCurrentTime = GameCurrentTime - GameStartedTime;
      GameCurrentTime = (LONGLONG) ( GameCurrentTime * Demoframelag );
    }
  }
#endif

#ifdef DEBUG_ON
  if ( framelag > 10.0F ) // check framelag out of reasonable range -> probably debugging
  {
    framelag = 10.0F; // clamp framelag to something reasonable
  }
#endif


/*===================================================================
  Procedure :  Main Routines to be called before Rendering....  
===================================================================*/

  InitIndirectVisible( Ships[Current_Camera_View].Object.Group );

  if( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) )
    AddIndirectVisible( (uint16) ( ( ActiveRemoteCamera ) ? ActiveRemoteCamera->Group : SecBulls[ CameraMissile ].GroupImIn ) );

  MainRoutines();

  if( MyGameStatus == STATUS_QuitCurrentGame )
    return TRUE;

  memset( (void*) &IsGroupVisible[0] , 0 , MAXGROUPS * sizeof(uint16) );
  cral += (framelag*2.0F);

  for( i = 0 ; i < MAX_SFX ; i++ )
    LastDistance[i] = 100000.0F;

/*===================================================================
  Procedure :  Now the Rendering can begin...
===================================================================*/

	if (!FSBeginScene())
		return FALSE;

	// show the stats screen
   if(ShowStats)
   {
	  ScoreDisplay();
   }

   // show regular view
   else if( !FullRearView )
   {
      CameraRendering = CAMRENDERING_Main;
      MainCamera.enable = 1;
      MainCamera.GroupImIn = Ships[Current_Camera_View].Object.Group; 
      MainCamera.Mat = Ships[Current_Camera_View].Object.FinalMat;  
      MainCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat;  
      MainCamera.Pos = Ships[Current_Camera_View].Object.Pos; 
      MainCamera.Viewport = viewport; 
      MainCamera.Proj = proj; 

      CurrentCamera = MainCamera;

      HUDNames();

      CurrentCamera.UseLowestLOD = FALSE;

      if( StereoEnabled )
      {
        cam_offset.x = StereoEyeSep / 2.0f;
        cam_offset.y = 0.0f;
        cam_offset.z = 0.0f;
        ApplyMatrix( &CurrentCamera.Mat, &cam_offset, &cam_offset );

        StereoMode = ST_LEFT;
        SetFOV( hfov );
        FSSetProjection( &proj );
        CurrentCamera.Pos.x -= cam_offset.x;
        CurrentCamera.Pos.y -= cam_offset.y;
        CurrentCamera.Pos.z -= cam_offset.z;
        render_set_filter( 1, 0, 0 );
        if( !RenderCurrentCamera() )
          return FALSE;
        StereoMode = ST_RIGHT;
        SetFOV( hfov );
        FSSetProjection( &proj );
        CurrentCamera.Pos.x += 2.0f * cam_offset.x;
        CurrentCamera.Pos.y += 2.0f * cam_offset.y;
        CurrentCamera.Pos.z += 2.0f * cam_offset.z;
        switch( StereoRightColor )
        {
        case ST_GREEN:
          render_set_filter( 0, 1, 0 );
          break;
        case ST_BLUE:
          render_set_filter( 0, 0, 1 );
          break;
        default:
          render_set_filter( 0, 1, 1 );
          break;
        }
        if( !RenderCurrentCamera() )
          return FALSE;
        StereoMode = ST_CENTER;
        SetFOV( hfov );
        FSSetProjection( &proj );
        CurrentCamera.Pos.x -= cam_offset.x;
        CurrentCamera.Pos.y -= cam_offset.y;
        CurrentCamera.Pos.z -= cam_offset.z;
        render_set_filter( 1, 1, 1 );
      }

	  // non stereo - normal rendering
      else
	  {
		if( RenderCurrentCamera() != TRUE ) // bjd
			return FALSE;
	  }
  
      if( RearCameraActive && !RearCameraDisable )
      {
        CameraRendering = CAMRENDERING_Rear;

        CurrentCamera.enable = 1;
        CurrentCamera.UseLowestLOD = TRUE;
        CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
        CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
        CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
        CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
        CurrentCamera.Viewport = viewport;  
        CurrentCamera.Proj = proj;  
        CurrentCamera.InvMat._31 *= -1.0F;
        CurrentCamera.InvMat._32 *= -1.0F;
        CurrentCamera.InvMat._33 *= -1.0F;
        CurrentCamera.InvMat._11 *= -1.0F;
        CurrentCamera.InvMat._12 *= -1.0F;
        CurrentCamera.InvMat._13 *= -1.0F;
        CurrentCamera.Mat._13 *= -1.0F;
        CurrentCamera.Mat._23 *= -1.0F;
        CurrentCamera.Mat._33 *= -1.0F;
        CurrentCamera.Mat._11 *= -1.0F;
        CurrentCamera.Mat._21 *= -1.0F;
        CurrentCamera.Mat._31 *= -1.0F;
        CurrentCamera.Viewport.X = ( viewport.X + viewport.Width ) - ( (viewport.Width >>4) + ( viewport.Width >>2 ) );
        CurrentCamera.Viewport.Y = viewport.Y + (viewport.Height >>4);
        CurrentCamera.Viewport.Width = viewport.Width >>2;
        CurrentCamera.Viewport.Height = viewport.Height >>2;
        CurrentCamera.Viewport.ScaleX = CurrentCamera.Viewport.Width / (float)2.0;
        CurrentCamera.Viewport.ScaleY = CurrentCamera.Viewport.Height / (float)2.0;

/* bjd
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwWidth),
                           RENDERVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwHeight),
                           RENDERVAL(2 * CurrentCamera.Viewport.dvScaleY));
*/        
        CurrentCamera.UseLowestLOD = TRUE;

        if( RenderCurrentCamera() != TRUE ) // bjd
            return FALSE;
      }

      if( ActiveRemoteCamera || (MissileCameraActive && MissileCameraEnable) )
      {
        float main_fov;

        TempMissileCam = Current_Camera_View;
        Current_Camera_View = -1;
        if( ActiveRemoteCamera )
        {
          CameraRendering = CAMRENDERING_Pip;
          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = ActiveRemoteCamera->Group;  
          CurrentCamera.Mat = ActiveRemoteCamera->Mat;  
          CurrentCamera.InvMat = ActiveRemoteCamera->InvMat;  
          CurrentCamera.Pos = ActiveRemoteCamera->Pos;
        }
		else
		{
          CameraRendering = CAMRENDERING_Missile;

          CurrentCamera.enable = 1;
          CurrentCamera.GroupImIn = SecBulls[ CameraMissile ].GroupImIn;  
          CurrentCamera.Mat = SecBulls[ CameraMissile ].Mat;  
          MatrixTranspose( &SecBulls[ CameraMissile ].Mat, &CurrentCamera.InvMat );
          CurrentCamera.Pos = SecBulls[ CameraMissile ].Pos;
        }
        CurrentCamera.Viewport = viewport;
        main_fov = hfov;
        SetFOV( normal_fov ); // was SetFOV( START_FOV ), but this doesn't work for wide angle fov
        CurrentCamera.Proj = proj;  
        
        CurrentCamera.Viewport.X = viewport.X + (viewport.Width >>4);
        CurrentCamera.Viewport.Y = viewport.Y + (viewport.Height >>4);
        CurrentCamera.Viewport.Width = viewport.Width >>2;
        CurrentCamera.Viewport.Height = viewport.Height >>2;
        CurrentCamera.Viewport.ScaleX = CurrentCamera.Viewport.Width / (float)2.0;
        CurrentCamera.Viewport.ScaleY = CurrentCamera.Viewport.Height / (float)2.0;
/* bjd 
        CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.wWidth),
                           RENDERVAL(2 * CurrentCamera.Viewport.dvScaleX));
        CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.Height),
                           RENDERVAL(2 * CurrentCamera.Viewport.dvScaleY));
*/        
        
        CurrentCamera.UseLowestLOD = TRUE;

        if( RenderCurrentCamera() != TRUE ) // bjd
            return FALSE;
       
        Current_Camera_View=TempMissileCam;
        SetFOV( main_fov );

      }
    }
	
    // Full Screen Rear View....
	else
	{
      CameraRendering = CAMRENDERING_Rear;
      CurrentCamera.enable = 1;
      CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
      CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
      CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
      CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
      CurrentCamera.Viewport = viewport;  
      CurrentCamera.Proj = proj;  
      CurrentCamera.InvMat._31 *= -1.0F;
      CurrentCamera.InvMat._32 *= -1.0F;
      CurrentCamera.InvMat._33 *= -1.0F;
      CurrentCamera.InvMat._11 *= -1.0F;
      CurrentCamera.InvMat._12 *= -1.0F;
      CurrentCamera.InvMat._13 *= -1.0F;
      CurrentCamera.Mat._13 *= -1.0F;
      CurrentCamera.Mat._23 *= -1.0F;
      CurrentCamera.Mat._33 *= -1.0F;
      CurrentCamera.Mat._11 *= -1.0F;
      CurrentCamera.Mat._21 *= -1.0F;
      CurrentCamera.Mat._31 *= -1.0F;
      CurrentCamera.UseLowestLOD = FALSE;

      if( RenderCurrentCamera() != TRUE ) // bjd
          return FALSE;

    }
 /* done with rendering camera stuff */
  
  /* do the target c omputer trick */
  if( TargetComputerOn )
  {
		set_alpha_states();
		DispTracker(); // bjd
  }

  if (!FSEndScene())
        return FALSE;

  ScreenPolyProcess();
  DispHUDNames();
  DispUntriggeredNMEs();

#ifdef INSIDE_BSP
  Inside = PointInsideSkin( &Ships[WhoIAm].Object.Pos, Ships[WhoIAm].Object.Group );
#endif

  if( Our_CalculateFrameRate() != TRUE)
      return FALSE;

/* Secondary routines called after rendering */
    
  if( !PlayDemo )
    NetworkGameUpdate();

  if( (Ships[WhoIAm].Object.Speed.z) > (MaxMoveSpeed) )
  {
    float diff;
    diff = ( Ships[WhoIAm].Object.Speed.z - MaxMoveSpeed ) / (  MaxTurboSpeed - MaxMoveSpeed );

    if( diff > 0.0F )
    {
      fov_inc += diff * diff * framelag;
      if ( fov_inc > 30.0F )
        fov_inc = 30.0F;
    }
    else
    {
      fov_inc *= (float) pow( 0.95, framelag );
    }
  }
  else
  {
    fov_inc *= (float) pow( 0.95, framelag );
  }
  SetFOV( chosen_fov + fov_inc );

    if( CurrentMenu && CurrentMenuItem )
    {
      MenuDraw( CurrentMenu );
      MenuItemDrawCursor( CurrentMenuItem );
	  DrawSimplePanel();
	  // Just to make sure that another press of escape doesnt take you back into the menu you wanted to exit!!
      JustExitedMenu = TRUE;
	  // menu keys are processed here
      MenuProcess();
    }
    else
    {
        if( !FullRearView )
          DrawSimplePanel();
    }

  // here is where we process F keys
  ProcessGameKeys();

  CheckLevelEnd();

  return TRUE;
}


/*===================================================================
  Procedure :   Show In Game Stats...
  Input   :   nothing...
  Output    :   nothing..
===================================================================*/

void ShowDeathModeStats()
{
	ShowGameStats( BOX_BG );
	if( Ships[ WhoIAm ].Timer < RESPAWN_TIMER )
		CenterPrint4x5Text( "Press any key to continue" , render_info.window_size.cy - (FontHeight*2) , GREEN );	
}

/* Display the Statistics in-game when key is pressed */
void ShowInGameStats()
{
	ShowGameStats( NO_BG );	// don't use BLT background
}

void PaintBackground( rect_t * box ) // pass NULL to black out all of the screen
{
	/* would simply blit the area defined by box on the backbuffer to black */
}

BOOL StatsNamePulse( void )
{
	static float pulse = 0.0F;
	pulse += framelag / 60.0F;

	if (pulse > 1.0F)
		pulse -= (float) floor( (double) pulse );

	if (pulse <= 0.5F)
		return TRUE;

	return FALSE;
}

extern int GetPlayerByRank( int rank );
extern int ReliabilityTab[MAX_PLAYERS+1];
void ShowGameStats( stats_mode_t mode )
{
	int active_players = 0;
	int total_rows = 0;
	int total_height = 0;
	int top_offset = 0;
	int row_height = (FontHeight+(FontHeight/2));
	int x_center = ( render_info.window_size.cx >>1 );
	int y_center = ( render_info.window_size.cy >>1 );

	// generate active players

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		if ( scoreable_status(i) )
			active_players++;
	}

	// get the total height of everything

	total_rows = 9 + (2* active_players); // calculated statically based on the code  SUCKSSSSS
	total_height = (total_rows * row_height);
	top_offset = ( y_center - (total_height / 2) ); // center whole thing

// ROWS += 1

	// paint black background or black box

	if( 0 && mode != NO_BG ) // TODO - could use flashbackgroundcolor function here if we want later
	{
		rect_t box;
		if( mode == BOX_BG )
		{
			int x_padding = FontWidth*2;
			int y_padding = FontHeight*2;

			// whole width
			box.left = 0;
			box.right = render_info.window_size.cx;

			//box.left = left_offset - x_padding;						// begging of first col
			//box.right = column[ ncols-1 ] + col_width + x_padding;	// end of last col

			box.top = top_offset - y_padding;							// begging of first row
			box.bottom = top_offset + total_height + y_padding;			// bottom of last row
		}
		PaintBackground(&box);
	}

	//
	// Kill Matrix
	//

	{
		char* title = "KILL MATRIX";
		Print4x5Text( title, (x_center-((strlen(title)*FontWidth)/2)), top_offset, WHITE );
		top_offset += (row_height * 2);
	}

// ROWS += 2

	{
		int i;

		int col_width = (FontWidth * 3);
		int name_width = (FontWidth * MAXSHORTNAME);

		int line_width = name_width + (col_width * active_players);
		int left_offset = x_center - (line_width / 2); // center

		// print first letter of each name

		for (i = 0; i < active_players; i++)
		{
			// x axis
			int xpos = left_offset + name_width + (i * col_width);

			// create string of first letter of player name
			char FirstLetter[2];
			FirstLetter[0] = Names[GetPlayerByRank(i)][0];
			FirstLetter[1] = 0;

			//
			Print4x5Text( FirstLetter,  xpos, top_offset, player_left(GetPlayerByRank(i)) ? DARKGRAY : GRAY );
		}

		top_offset += row_height;

// ROWS += 1

		// display kill matrix
		for (i = 0; i < active_players; i++)
		{
			// reset value
			int j = 0;
			int xpos = left_offset;

			// print name
			Print4x5Text( (char*) &Names[GetPlayerByRank(i)], xpos, top_offset, GRAY );
			xpos += name_width;

			// print kill matrix line
			for (j = 0; j < active_players; j++)
			{
				int color = player_left(GetPlayerByRank(i)) ? DARKGRAY : ((i==j) ? RED : GRAY);
				Printint16( GetKillStats(GetPlayerByRank(i),GetPlayerByRank(j)), xpos, top_offset, color ); // suacides are red
				xpos+=col_width; // next col
			}

			// next row
			top_offset += row_height;
		}

// ROWS += active players

	}

	top_offset += (row_height * 4);

// ROWS += 4

	//
	// Player Stats
	//

	{
		char* title = "Player Stats";
		Print4x5Text( title, (x_center-((strlen(title)*FontWidth)/2)), top_offset, WHITE );
		top_offset += (row_height * 2);
	}

// ROWS += 2


	{
		int i;

		int col_width = (FontWidth * MAXSHORTNAME);
		int ncols = (TeamGame) ? 7 : 6;
		int line_width = col_width * ncols;
		int left_offset = x_center - (line_width / 2); // center

		{
			int xpos;
			int col;

			// names and colors
			int columns = 6;
			char* names[6] = {"PING", "RATIO", "TEAM", "OVERALL", "KILLS", "DEATHS"};
			int colors[6]  = {  GRAY,   CYAN,  YELLOW,      GRAY,   GREEN,      RED};

			// NOT team game
			if(!TeamGame)
				names[2] = NULL;

			// start at left
			xpos = left_offset;

			// print the names
			for( col = 0; col < columns; col++ )
				if( names[col] != NULL )
					Print4x5Text( names[col], (xpos+=col_width), top_offset, colors[col] );

		}

		top_offset += (row_height * 1);

// ROWS += 1

		for (i = 0; i < active_players; i++)
		{
			// ping
			uint16 ping = 0;

			// start of line
			int xpos = left_offset;

			// if the player left the game
			int left = player_left(GetPlayerByRank(i));

			// calculate name color
			int color = YELLOW;													// default name color is yellow
			if (StatsNamePulse() && GetPlayerByRank(i) == WhoIAm)					// flash player name
				color = GRAY;
			else if (TeamGame)													// set color to team color
				color = TeamCol[TeamNumber[GetPlayerByRank(i)]];

			// get the ping
			if( Ships[GetPlayerByRank(i)].network_player )
				ping = (uint16) Ships[GetPlayerByRank(i)].network_player->ping;

			//
			// print line
			//
			
			// connection status
			// print the dot about 2 character space gap from the names
			if( ! left )
				DisplayConnectionStatus( ReliabilityTab[GetPlayerByRank(i)], (xpos-(4*FontWidth)), top_offset );

			// name
			Print4x5Text( (char*)&Names[GetPlayerByRank(i)],	xpos,			top_offset, (left) ? DARKGRAY : color );

			// add width now so the ping column shows up even if a value printed
			xpos += col_width;

				if( GetPlayerByRank(i) != WhoIAm && GameStatus[GetPlayerByRank(i)] == STATUS_Normal )
			Printint16( ping,								xpos,				top_offset, (left) ? DARKGRAY : GRAY	);	// ping
			Printint16( GetEffeciency(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset, (left) ? DARKGRAY : CYAN	);	// positives / (positives - negatives)
				if(TeamGame)
			Printint16( GetTeamScore(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset,	(left) ? DARKGRAY : YELLOW	);	// all players (points + kills - suacides - friendly - deaths)
			Printint16( GetRealScore(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset, (left) ? DARKGRAY : GRAY	);	// points + kills - suacides - friendly - deaths
			Printint16( GetKills(GetPlayerByRank(i)),		(xpos+=col_width),	top_offset,	(left) ? DARKGRAY : GREEN	);	// kills - suacides - friendly
			Printint16( GetTotalDeaths(GetPlayerByRank(i)),	(xpos+=col_width),	top_offset,	(left) ? DARKGRAY : RED		);	// suacides + deaths

			// go to next row
			top_offset += row_height;
		}
	}

// ROWS += active players

}

/*===================================================================
  Procedure :   Score Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
===================================================================*/
/* Display the Statistics */
BOOL ScoreDisplay()
{
	ShowGameStats( FULL_BG ); // use BLT background
	CenterPrint4x5Text( "Press Space to continue" , render_info.window_size.cy - (FontHeight*2) , 0 );	
	return TRUE;
}


/*===================================================================
  Procedure :   Init Score Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
===================================================================*/
BOOL  InitScoreDisplay()
{
#ifdef SCROLLING_MESSAGES
  int i;
#endif
   //Create the offscreen surface, by loading our bitmap.

  InitFont();
  //ChangeBackgroundColour( 0, 0, 0 );

#ifdef SCROLLING_MESSAGES
  // get random phrases...
  for (i = 0; i < Num_StatsMessage_Parts; i++)
  {
    StatsMessages[i].current_variation = Random_Range(StatsMessages[i].num_variations);
  }
#endif
  // process scores...
  ScoreSort();
  ScoreDisplaySfx = TRUE;

  return TRUE;
}

void ScrollingTeamMessage(char **str, int num_strings, int *col)
{
  static uint16 StartChar = 0;
  static float TotalShift = 0;
  char *message;
  int MaxChars, i;
  uint16 CurrentChar;
  int TotalLength;
  uint16 CurrentMessagePos, ProcessedChars, CurrentCharOffset, CurrentString, CurrentStrLen;
  int xpos;

  TotalShift += 0.5F * framelag;

  TotalLength = 0;
  for (i = 0; i < num_strings; i++)
    TotalLength += strlen( str[i] );

  if (StartChar > (TotalLength - 1))
    StartChar = 0;

  while( TotalShift >= FontWidth )
  {
    StartChar++;
    if (StartChar == TotalLength)
      StartChar = 0;
    TotalShift -= FontWidth;
  }

  MaxChars = (render_info.window_size.cx / FontWidth) + 2;

  message = (char *)calloc( MaxChars, sizeof(char) );
  
  CurrentChar = StartChar;
  CurrentMessagePos = 0;
  ProcessedChars = 0;
  xpos = 0 - (int)TotalShift;

  while( ProcessedChars++ <= MaxChars )
  {
    // find out which string CurrentChar is in...
    CurrentCharOffset = 0;
    for (CurrentString = 0; CurrentString < num_strings; CurrentString++)
    {
      if ( CurrentChar < strlen( str[CurrentString] ) + CurrentCharOffset )
      {
        CurrentStrLen = strlen( str[CurrentString] );
        break;
      }
      else
        CurrentCharOffset += strlen( str[CurrentString] );
    }

    // get next character from string, and add to message...
    message[CurrentMessagePos++] = str[CurrentString][CurrentChar++ - CurrentCharOffset];

    if ( (CurrentChar - CurrentCharOffset) == CurrentStrLen )
    {
      // display text so far...
      message[CurrentMessagePos] = 0;
      PrintClipped4x5Text( message, xpos, render_info.window_size.cy - (FontHeight*2) , col[CurrentString] );
      xpos += FontWidth * strlen( message );
      message[0] = 0;
      CurrentMessagePos = 0;

      if (CurrentString == num_strings - 1)
        CurrentChar = 0;
    }
  }
  // display remaining text...
  message[CurrentMessagePos] = 0;
  PrintClipped4x5Text( message, xpos, render_info.window_size.cy - (FontHeight*2) , col[CurrentString] );

  free (message);
}

// Clears the target(back) and zbuffer for the current camera
BOOL ClearBuffers( void )
{
	XYRECT dummy;

	if (!render_info.ok_to_render)
		return FALSE;

	dummy.x1 = CurrentCamera.Viewport.X;
	dummy.y1 = CurrentCamera.Viewport.Y;
	dummy.x2 = CurrentCamera.Viewport.X + CurrentCamera.Viewport.Width;
	dummy.y2 = CurrentCamera.Viewport.Y + CurrentCamera.Viewport.Height;

	return FSClear(&dummy);
}

// Clear the Zbuffer
BOOL ClearZBuffer()
{
	XYRECT dummy;

	if (!render_info.ok_to_render)
		return FALSE;

	dummy.x1 = CurrentCamera.Viewport.X;
	dummy.y1 = CurrentCamera.Viewport.Y;
	dummy.x2 = CurrentCamera.Viewport.X+CurrentCamera.Viewport.Width;
	dummy.y2 = CurrentCamera.Viewport.Y+CurrentCamera.Viewport.Height;

	return FSClearDepth(&dummy);
}


void InitRenderBufs(/* LPDIRECT3DDEVICE lpDev */) // bjd
{
	ReleaseRenderBufs();
	// just vertex data
	FSCreateDynamicVertexBuffer(&RenderBufs[0], 32767);
	FSCreateDynamicVertexBuffer(&RenderBufs[1], 32767);
	// index data as well
	FSCreateDynamicVertexBuffer(&RenderBufs[2], 32767);
	FSCreateDynamicIndexBuffer(&RenderBufs[2], 32767*3);
	// indexed pre-transformed (2d)
	FSCreateDynamic2dVertexBuffer(&RenderBufs[3], 32767);
	FSCreateDynamicIndexBuffer(&RenderBufs[3], 32767*3);
}

void ReleaseRenderBufs( void )
{
	FSReleaseRenderObject(&RenderBufs[0]);
	FSReleaseRenderObject(&RenderBufs[1]);
	FSReleaseRenderObject(&RenderBufs[2]);
	FSReleaseRenderObject(&RenderBufs[3]);
}


/*===================================================================
  Procedure :  Render 1 Frame Using CurrentCamera...
  Input   :
  Output    : BOOL TRUE/FALSE
===================================================================*/
BOOL RenderCurrentCamera( void )
{
	int16 Count;
	VISGROUP  *g;
	uint16  group;
//	float R, G, B;
	NumOfTransExe = 0;

	Build_View();
	CurrentCamera.View = view;

	if (!FSSetView(&view))
		return FALSE;

    if (!FSSetViewPort(&CurrentCamera.Viewport)) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera1", &CurrentCamera.Viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return FALSE;
    }

  // Ship Model Enable/Disable
  SetShipsVisibleFlag();

  // find visible groups
  FindVisible( &CurrentCamera, &Mloadheader );

  BuildVisibleLightList( CurrentCamera.GroupImIn );
  
  UpdateBGObjectsClipGroup( &CurrentCamera );
  UpdateEnemiesClipGroup( &CurrentCamera );

  /*
  if( CurrentCamera.GroupImIn != (uint16) -1 )
  {
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
      if( Mloadheader.Group[ g->group ].BGClear_Flag )
      {
        if ( Mloadheader.Group[ g->group ].BGClear_Flag & 0x80 )
        {
          R = Mloadheader.Group[ g->group ].BGClear_Red;
          G = Mloadheader.Group[ g->group ].BGClear_Green;
          B = Mloadheader.Group[ g->group ].BGClear_Blue;
          ChangeBackgroundColour( R, G, B );
        }
        break;
      }
    }
  }
  */

  if (ClearBuffers() != TRUE )
    return FALSE;

	// reset all the normal execute status flags...
	set_normal_states();

	// set all the Translucent execute status flags...
	if( WhiteOut != 0.0F)
	{
		set_alpha_fx_states();
	}


	// display background
	if ( !DisplayBackground( &Mloadheader, &CurrentCamera ) )
		return FALSE;

	// reset all the normal execute status flags...
	if( WhiteOut == 0.0F)
	{
		set_normal_states();
	}

/*===================================================================
  Display Non Group Clipped Non Faceme Transluecent Polys
===================================================================*/
    if( !DisplaySolidGroupUnclippedPolys( &RenderBufs[ 2 ] ) ) // bjd
        return FALSE;

#ifdef SHADOWTEST
    if( !DisplaySolidGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
        return FALSE;
#endif
  
  // display clipped opaque objects
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;

    // Do the Background animation for that group.....
    BackGroundTextureAnimation( &Mloadheader , group );

#ifdef CLIP_LINES
    ClipGroup( &CurrentCamera, group );
#else
    ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );
#endif

	ExecuteLines( group, &RenderBufs[ 0 ] );

    ClipGroup( &CurrentCamera, group );

      if( !ModelDisp( group, /*lpDev,*/ &ModelNames[0] ) ) // bjd
        return FALSE;

/*===================================================================
  Display Group Clipped Non Faceme Transluecent Polys
===================================================================*/

  if( !DisplaySolidGroupClippedPolys( &RenderBufs[ 2 ], group ) ) // bjd
    return FALSE;
#ifdef SHADOWTEST
  if( !DisplaySolidGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;
#endif

  }

  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );

	// set all the Translucent execute status flags...
  	set_alpha_states();


/*===================================================================
  Display Skin & BSP Node Info
===================================================================*/
  if( ShowSkin || OldNodeCube || NodeCube || ShowTrigZones || ShowColZones || ShowEFZones || ShowTeleports )
  {
	if( OldNodeCube || NodeCube )
		ClearZBuffer();

	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
//		if( Skin_Execs[ Count ] != NULL )
		{
			if (!draw_object(&Skin_Execs[ Count ]))
			{
				return FALSE;
			}
		}
	}

#if 0
    for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
    {
		group = g->group;
		ClipGroup( &CurrentCamera, group );
		ExecuteLines( group, &RenderBufs[ 0 ] );
    }
#endif

  }

		set_alpha_states();

  // display clipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ClipGroup( &CurrentCamera, group );
    GroupWaterProcessDisplay( group );



/*===================================================================
  Display Group Clipped Non Faceme Transluecent Polys
===================================================================*/

  if( !DisplayGroupClippedPolys( &RenderBufs[ 2 ], group ) ) // bjd
    return FALSE;

#ifdef SHADOWTEST
  if( !DisplayGroupClippedTriangles( RenderBufs[ 1 ], group, lpDev, lpView ) )
    return FALSE;
#endif

/*===================================================================
Display Group Clipped Faceme Transluecent Polys
===================================================================*/

  if( !DisplayGroupClippedFmPolys( &RenderBufs[ 2 ], group ) ) // bjd
      return FALSE;

  ExecuteTransExe( group );

  }


  ClipGroup( &CurrentCamera, CurrentCamera.GroupImIn );


/*===================================================================
  Display Non Group Clipped Faceme Transluecent Polys
===================================================================*/

    if( !DisplayGroupUnclippedFmPolys( &RenderBufs[ 2 ] ) ) // bjd
        return FALSE;

/*===================================================================
  Display Non Group Clipped Non Faceme Transluecent Polys
===================================================================*/
    if( !DisplayGroupUnclippedPolys( &RenderBufs[ 2 ] ) ) // bjd
        return FALSE;
#ifdef SHADOWTEST
    if( !DisplayGroupUnclippedTriangles( RenderBufs[ 0 ], lpDev, lpView ) )
      return FALSE;
#endif
  
  // display unclipped translucencies
  for ( g = CurrentCamera.visible.first_visible; g; g = g->next_visible )
  {
    group = g->group;
    ExecuteTransExeUnclipped( group );
  }


/*===================================================================
  Display Portals
===================================================================*/
  if( ShowPortal )
  {
	for( Count = 0; Count < MAXGROUPS; Count++ )
	{
//		if( Portal_Execs[ Count ] != NULL )
		{
			if (!draw_object(&Portal_Execs[ Count ]))
			{
				return FALSE;
			}
		}
	}
  }
  
  DoLensflareEffect();
  DoAllSecBullLensflare();

/*===================================================================
  Display Transluecent Screen Polys
===================================================================*/

    if( !DisplayNonSolidScrPolys( &RenderBufs[ 3 ] ) )
      return FALSE;

//
// turn off transparency modes
//

	set_normal_states();

/*===================================================================
  Display Solid Screen Polys
===================================================================*/

  if( !DisplaySolidScrPolys( &RenderBufs[ 3 ] ) )
    return FALSE;

    if (!FSSetViewPort(&viewport)) {
#ifdef DEBUG_VIEWPORT
    SetViewportError( "RenderCurrentCamera2", &viewport );
#else
        Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
        return FALSE;
    }

  return TRUE;
}
  

extern  int   NumOfVertsConsidered;
extern  int   NumOfVertsTouched;

/*===================================================================
  Procedure :  Our Calculate frame rate...
  Input   :
  Output    : BOOL TRUE/FALSE
===================================================================*/

px_timer_t our_timer;
float our_count = 0;
extern BOOL ShowFrameRate;
extern BOOL ShowInfo;
extern	uint16		NumGroupsVisible;
extern uint16	GroupImIn;

BOOL Our_CalculateFrameRate(void)
{
	char buf[256];
	static int avg_time_per_frame = 0;

	// every 25 frames
	our_count++;
	if (our_count >= 30) // every 0.5 seconds on vsync pc
	{
		// calculate time passed
		float seconds = timer_run( &our_timer );

		// first time running so bank
		if ( seconds != 0.0F )
		{
			// calculate average frames per second
			FPS = our_count / seconds;

			// average time per frame in milliseconds
			avg_time_per_frame = (int)((1.0F / FPS) * 1000.0F);

			//
			our_count = 0;
		}
	}
  
	// display the framerate
	if( ShowFrameRate )
	{
		sprintf(&buf[0], "FPS %d - AVG F %d MS", (int) FPS, avg_time_per_frame );
		CenterPrint4x5Text( (char *) &buf[0] , FontHeight, 2 );
	}

	if( ShowInfo )
	{

		// group information
		sprintf(&buf[0], "Groups Visible %d - Current Group %s", (int) NumGroupsVisible,
			(GroupImIn == (uint16) -1) ? "(outside)" : Mloadheader.Group[GroupImIn].name );
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*3, 2 );

		// memory information
		sprintf(&buf[0], "Mem %d",(int)MemUsed );
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*4, 2 );

		// show polygon information
		sprintf(&buf[0], "Face Me Polys %d - Dynamic? Polys %d - Screen Polys %d - Verts Touched (lighting?) %d",
			(int) TotalFmPolysInUse,(int) TotalPolysInUse,(int) TotalScrPolysInUse, NumOfVertsTouched);
		CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*6, 2 );

		if ( ! ShowWeaponKills ) //ShowNetworkInfo)
		{

			// newtork info
			sprintf( &buf[0], "Network" );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*8, 2 );

			sprintf( &buf[0], "BPS %5d CurSent %5d MaxRec %5d MaxSent %5d",
				(int)CurrentBytesPerSecRec , (int)CurrentBytesPerSecSent , (int)MaxCurrentBytesPerSecRec , (int)MaxCurrentBytesPerSecSent );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*9, 2 );

			sprintf( &buf[0], "ReceiveSize %4d MaxReceiveSize %4d", (int)RecPacketSize , (int)MaxRecPacketSize );
			CenterPrint4x5Text( (char *) &buf[0], (FontHeight+3)*10, 2 );

		}

		if (ShowWeaponKills)
		{
			// show all primary weapon kills
			for( i = 0 ; i < MAXPRIMARYWEAPONS+1 ; i++ )
			{
				// display primary weapon name
				Print4x5Text( GetWeaponName(WEPTYPE_Primary,i),	(render_info.window_size.cx>>1)-(11*FontWidth),	(viewport.Y + (viewport.Height>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
				// display primary weapon kills
				Printuint16( GetWeaponKillStats(WhoIAm,WEPTYPE_Primary,i),	(render_info.window_size.cx>>1)-(15*FontWidth), (viewport.Y + (viewport.Height>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
			}

			// show all secondary weapon kills
			for( i = 0 ; i < TOTALSECONDARYWEAPONS ; i++ )
			{		
				// display secondary weapon name
				Print4x5Text( GetWeaponName(WEPTYPE_Secondary,i),	(render_info.window_size.cx>>1)+(5*FontWidth),	(viewport.Y + (viewport.Height>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
				// display secondary weapon kills
				Printuint16( GetWeaponKillStats(WhoIAm,WEPTYPE_Secondary,i) , (render_info.window_size.cx>>1)+(1*FontWidth),	(viewport.Y + (viewport.Height>>2))+( i * ( FontHeight+(FontHeight>>1) ) ), 2 );
			}
		}

	}

#ifdef DEMO_SUPPORT
    // some stupid place for a demo calculation
	if( MyGameStatus == STATUS_PlayingDemo )
	{
		QueryPerformanceFrequency((LARGE_INTEGER *) &Freq);
		QueryPerformanceCounter((LARGE_INTEGER *) &DemoEndedTime);
		TimeDiff = DemoEndedTime - DemoStartedTime;
		DemoTotalTime = ( (float) TimeDiff / (float) Freq );
		DemoAvgFps = DemoGameLoops / DemoTotalTime;
	}
#endif

	return TRUE;
}

/*===================================================================
  Procedure : Init View port without calling initview..
  Input   : Nothing
  Output    : Nothing
===================================================================*/
BOOL
InitViewport( void )
{
	int left, top;
	int width, height;
	int maxwidth, maxheight;
	float scale = 1.1F;

	/*
	 * Setup the viewport for specified viewing area
	 */

	if (!FSGetViewPort(&viewport))
	{
		Msg( "GetViewport failed.\n%s", render_error_description(0) );
		return FALSE;
	}

	maxwidth = render_info.window_size.cx;
	maxheight = render_info.window_size.cy;

	width = maxwidth;
	height = maxheight;
	if ( scale > 1.11F )
	{
		DrawPanel = TRUE;
		scale = 1.2F;
	}
	else
	{
		DrawPanel = FALSE;
	}
	left = 0;
	top = 0;

    viewport.X = left;
	viewport.Y = top;
    viewport.Width = width;
    viewport.Height = height;
    viewport.ScaleX = viewport.Width / (float)2.0;
    viewport.ScaleY = viewport.Height / (float)2.0;

	if (!FSSetViewPort(&viewport))
	{
#ifdef DEBUG_VIEWPORT
		SetViewportError( "InitViewport", &viewport );
#else
		Msg("SetViewport failed.\n%s", render_error_description(0));
#endif
		return FALSE;
	}
	
	SetFOV( hfov );

	FSClearBlack();

	return TRUE;
}

/*===================================================================
  Procedure :   Special Destroy game for dplay fuck up..
  Input   :   nothing..
  Output    :   nothing..
===================================================================*/
void SpecialDestroyGame( void )
{
	IMustQuit = FALSE;

	DebugPrintf("SpecialDestroyGame()\n");

	if ( PlayDemo )
		PlayDemo = FALSE;

	switch( MyGameStatus )
	{
	case STATUS_WaitingToStartTeamGame:
	case STATUS_StartingMultiplayer:
	case STATUS_GetPlayerNum:
		MyGameStatus = STATUS_Title;
		MenuRestart( &MENU_ForceAbort );
		break;
	default:
		MyGameStatus = STATUS_QuitCurrentGame;
		break;
	}

	network_cleanup();
}
/*===================================================================
  Procedure :   Calculate the framelag..
  Input   :   nothing..
  Output    :   nothing..
===================================================================*/
void CalculateFramelag( void )
{

  // forsaken multiplies seconds * 71.0F
  // the idea of multiplying by anything here would be a base conversion
  // such as from seconds into ms or ns to instead of a long fraction

  // but 71.0F is not a power of 10 so this has really bad affects
  // you can end up with exponential varying results on diff systems

  // one loop sample on a 2GHZ P4 with 768MB of ram and an nivdia vanta
  // 0.00744508 * 71  = 0.52860068

  // lets say that computer B taks 50% longer to compute the game loop
  // 0.00744508 * 1.5 = 0.01116762

  // that means the calculation look like the following for computer B
  // 0.01116762 * 71  = 0.80406864

  // now our game loop timing is being tweaked differently on both pc's
  // the problem is where our game loop has a real difference of:
  // 0.00372254 seconds 1.5% apart

  // they are now this far apart
  // 0.27546796 seconds 1.5211267605633802816901408450704%

  // resulting in a drift per loop of
  // 0.27546796 - 0.00372254 = 0.27174542 seconds

  // now to get the number of loops in 1 second on the slower pc
  // 1 / 0.01116762 = 89.544594103309389108870108402686

  // the slower pc is adding the drift 89.5 times per second
  // 89.544594103309389108870108402686 * 0.27174542 = 24.333333333333333333333333333332

  // thus  internal clock between the two pc's will be altered by 24.3
  // with the slower pc being *ahead* of the faster one in game time
  
  // not sure on the meaning of the math here but it looks like
  // this should represent a change of +0.18th of a second
  // 24.3 * 0.007 = 0.18225

  // on top of all this fact is that this is just a single sample
  // pc's could perhaps very more than 1.5% at fluctuating rates
  // until we have some real data from all our pc's we won't really know

  // forsaken uses framelag (fraction) to multiply by other numbers
  // so the faster pc with a smaller fraction gets smaller results
  // meaning that a faster pc will evolve the world at smaller steps
  // and the slower pc you should see the world moving at faster steps

  // now this is only relavant to places that actually use this variable
  // but it could have impact on various time based world updates such as:
  // packes per second, pickup rotation, mine movement/shooting,
  // bikes speed, input reading, lighting etc...

  // thus all of the above would work faster on a slower pc...
  // or perhaps even that missiles could fly *slower* on faster machines!!!!

  // note that player positions are not affected
  // since they are constantly updated via networking
  // and are not time based at all....

  real_framelag  = timer_run( &framelag_timer );
  
  framelag = real_framelag * 71.0F;

  // debugging
  //if (TimeWorst == 0.0 || seconds > TimeWorst) TimeWorst = seconds;
  //if (TimeBest  == 0.0 || seconds < TimeBest)  TimeBest  = seconds;
  //DebugPrintf("Loop Time: seconds=%5f - worst:%5f - best:%5f\n",seconds,TimeWorst,TimeBest);
  
  //  who knows why
  if( CurrentMenu && (MyGameStatus == STATUS_SinglePlayer) )
    framelag = 0.0F;

}


/*===================================================================
  Procedure : Disp Tracker
  Output    : BOOL        TRUE/FALSE
===================================================================*/
BOOL DispTracker( void ) // bjd
{
	uint16      i;
	XYRECT     dummy;
	render_viewport_t newviewport;
	float       screen_width, screen_height;
	VECTOR      TempVector;
	MATRIX      TempMatrix;
	float       TargetDistance;
	float       Red, Green, Blue, Trans;
	VECTOR      ShipDir, TargetDir;
	float       Cos;

	if ( render_info.fullscreen )
	{
		screen_width = (float) render_info.ThisMode.w;
		screen_height = (float) render_info.ThisMode.h;
	}
	else
	{
		screen_width = (float) render_info.WindowsDisplay.w;
		screen_height = (float) render_info.WindowsDisplay.h;
	}

    newviewport.X = 0;	
	newviewport.Y = 0;
    newviewport.Width = ( render_info.window_size.cx / 3 ) & -2;
    newviewport.Height = (uint32) ( (float) newviewport.Width * pixel_aspect_ratio );
    newviewport.ScaleX = newviewport.Width / (float)2.0;
    newviewport.ScaleY = newviewport.Height / (float)2.0;

/* bjd
    newviewport.dvMaxX = (float)D3DDivide(RENDERVAL(newviewport.dwWidth),
                                       RENDERVAL(2 * newviewport.dvScaleX));
    newviewport.dvMaxY = (float)D3DDivide(RENDERVAL(newviewport.dwHeight),
                                       RENDERVAL(2 * newviewport.dvScaleY));
*/

	if (!FSSetViewPort(&newviewport))
		return FALSE;

	viewplane_distance = (float) ( newviewport.Width / ( 2 * tan( DEG2RAD( normal_fov ) * 0.5 ) ) );
	panelproj._11 = 2 * viewplane_distance / newviewport.Width;
	panelproj._22 = 2 * viewplane_distance / ( newviewport.Height / pixel_aspect_ratio );

	if (!FSSetProjection(&panelproj))
	{
		return FALSE;
	}

	dummy.x1 = newviewport.X;
	dummy.x2 = newviewport.X + newviewport.Width;
	dummy.y1 = newviewport.Y;
	dummy.y2 = newviewport.Y + newviewport.Height;

	if (FSClearDepth(&dummy))
		return FALSE;

	MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
	ShadeModel( MODEL_Tracker, &TempMatrix, ( 40.0F * GLOBAL_SCALE ), ( 220.0F * GLOBAL_SCALE ) );

	TempMatrix = MATRIX_Identity;
	TempMatrix._11 = 1.25F;
	TempMatrix._22 = 1.25F;
	TempMatrix._33 = 1.25F;
	MatrixMultiply( &TempMatrix, &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );

	view._11 = TempMatrix._11; 
	view._12 = TempMatrix._12;
	view._13 = TempMatrix._13;
	view._14 = TempMatrix._14;
	         
	view._21 = TempMatrix._21;
	view._22 = TempMatrix._22;
	view._23 = TempMatrix._23;
	view._24 = TempMatrix._24;
	         
	view._31 = TempMatrix._31;
	view._32 = TempMatrix._32;
	view._33 = TempMatrix._33;
	view._34 = TempMatrix._34;

	view._41 = 0.0F;
	view._42 = 0.0F;
	view._43 = ( 280.0F * GLOBAL_SCALE );
	view._44 = 1.0F;

	if (!FSSetView(&view))
		return FALSE;

	if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Tracker], (uint16) -1 ) != TRUE )
		return FALSE;

#if 0
  i = FindClosestPickup();
#else
  i = FindClosestShip();
#endif
  
  if( i != (uint16) -1 )
  {
    Red = 255.0F;
    Green = 255.0F;
    Blue = 255.0F;
    Trans = 255.0F;
#if 0
    TempVector.x = ( ( Pickups[ i ].Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Pickups[ i ].Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Pickups[ i ].Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#else
    TempVector.x = ( ( Ships[ i ].Object.Pos.x - Ships[ WhoIAm ].Object.Pos.x ) / 50.0F );
    TempVector.y = ( ( Ships[ i ].Object.Pos.y - Ships[ WhoIAm ].Object.Pos.y ) / 50.0F );
    TempVector.z = ( ( Ships[ i ].Object.Pos.z - Ships[ WhoIAm ].Object.Pos.z ) / 50.0F );
#endif
    TargetDistance = VectorLength( &TempVector );

    if( TargetDistance > ( 130.0F * GLOBAL_SCALE ) )
    {
      NormaliseVector( &TempVector );
      TempVector.x *= ( 130.0F * GLOBAL_SCALE );
      TempVector.y *= ( 130.0F * GLOBAL_SCALE );
      TempVector.z *= ( 130.0F * GLOBAL_SCALE );
      Green = 64.0F;
      Blue = 64.0F;
    }
    else
    {
      ApplyMatrix( &Ships[ WhoIAm ].Object.FinalMat, &Forward, &ShipDir );
      NormaliseVector( &ShipDir );
      TargetDir = TempVector;
      NormaliseVector( &TargetDir );
      Cos = DotProduct( &ShipDir, &TargetDir );
      if( Cos < 0.0F )
      {
        Red = 256.0F + ( Cos * 192.0F );
        Green = 256.0F + ( Cos * 192.0F );
      }
    }

    TintModel( MODEL_Ping, Red, Green, Blue, Trans );

    MatrixTranspose( &Ships[ WhoIAm ].Object.FinalMat, &TempMatrix );
    ApplyMatrix( &TempMatrix, &TempVector, &TempVector ); // Calc Up Vector

    view = identity;
    view._11 = 1.25F;
    view._22 = 1.25F;
    view._33 = 1.25F;
    view._41 = TempVector.x;
    view._42 = TempVector.y;
    view._43 = TempVector.z + ( 280.0F * GLOBAL_SCALE );
    view._44 = 1.0F;

	if (!FSSetView(&view))
		return FALSE;

    if (ReallyExecuteMxloadHeader( &ModelHeaders[MODEL_Ping], (uint16) -1 ) != TRUE )
      return FALSE;
  }

	if (!FSSetViewPort(&viewport))
		return FALSE;

	if (!FSSetProjection(&proj))
	{
		return FALSE;
	}

  return TRUE;
}


/*===================================================================
  Procedure : Change Background Colour
  Input   : float R ( 0.0F - 1.0F )
        : float G ( 0.0F - 1.0F )
        : float B ( 0.0F - 1.0F )
  Output    : BOOL  TRUE/FALSE
===================================================================*/
// TODO - might want this
/*
BOOL ChangeBackgroundColour( float R, float G, float B )
{
    RENDERMATERIAL bmat;
	memset(&bmat, 0, sizeof(RENDERMATERIAL));
	bmat.Diffuse.r = R;
	bmat.Diffuse.g = G;
	bmat.Diffuse.b = B;

	// bjd - this still needs to be done
	//       what did this use to do anyway?

	return TRUE;
}
*/

/*===================================================================
  Procedure : Print up how far the init view has to go...
  Input   : BYTE GameStatus
  Output    : void
===================================================================*/
void PrintInitViewStatus( BYTE Status )
{
	int i;
	RENDEROBJECT ro;
	FSCreateDynamic2dVertexBuffer(&ro, 32767);
	FSCreateIndexBuffer(&ro, 32767*3);
	for( i = 0 ; i < ( Status - STATUS_InitView_0 )+1 ; i ++ )
		CenterPrint4x5Text( InitViewMessages[i], ( render_info.window_size.cy >> 2 ) + ( i * ( FontHeight + ( FontHeight>>1 ) ) ) , GREEN );
	DisplayNonSolidScrPolys(&ro);
	DisplaySolidScrPolys(&ro);
	FSReleaseRenderObject(&ro);
	render_flip(&render_info);
}

/*===================================================================
  Procedure :   Init Stats Display Stuff...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
===================================================================*/
BOOL  InitStatsDisplay()
{
  // Create the offscreen surface, by loading our bitmap.

  InitFont();
  //ChangeBackgroundColour( 0, 0, 0 );

  HowManySecrets( &TotalSecrets , &Secrets );

  return TRUE;
}

/*===================================================================
  Procedure :   Stats Display...
  Input   :   nothing...
  Output    :   BOOL TRUE/FALSE
===================================================================*/
BOOL StatsDisplay()
{
  rect_t    src;
  rect_t    dest;
  point_t	destp;
  int x,y;

  char buf[256];

  //  Blt Background
  src.top = 0;
  src.left = 0;
  src.right = render_info.window_size.cx;
  src.bottom = render_info.window_size.cy;
  x = 0;
  y = 0;

  dest.top = 0;
  dest.bottom = render_info.window_size.cy;
  dest.left = 0;
  dest.right = render_info.window_size.cx;
      
  destp.x = dest.left;
  destp.y = dest.top;

  if( !GameCompleted )
  {
    if( Secrets == 1 )
    {
      sprintf( (char*) &buf ,"%d Secret found out of %d", Secrets , TotalSecrets );
    }else{
      sprintf( (char*) &buf ,"%d Secrets found out of %d", Secrets , TotalSecrets );
    }
    CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy >> 1 ) - (FontHeight*2) , 2 );
    if( NumKilledEnemies == 1 )
    {
      sprintf( (char*) &buf ,"%d Enemy killed out of %d", NumKilledEnemies , NumInitEnemies );
    }else{
      sprintf( (char*) &buf ,"%d Enemies killed out of %d", NumKilledEnemies , NumInitEnemies );
    }
    CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy >> 1 ) + (FontHeight*0) , 2 );
    if( CrystalsFound == 1 )
    {
      sprintf( (char*) &buf ,"%d Crystal found so far", CrystalsFound );
    }else{
      sprintf( (char*) &buf ,"%d Crystals found so far", CrystalsFound );
    }
    CenterPrint4x5Text( &buf[0] , (render_info.window_size.cy >> 1 ) + (FontHeight*2) , 2 );
  }else{
    CenterPrint4x5Text( "Congratulations" , (render_info.window_size.cy >> 1 ) - (FontHeight*2) , 2 );
    CenterPrint4x5Text( "Demo Complete" , (render_info.window_size.cy >> 1 ) + (FontHeight*0) , 2 );
    if( DifficultyLevel != 3 )
    {
      CenterPrint4x5Text( "Now Try a Harder Difficulty Setting" , (render_info.window_size.cy >> 1 ) + (FontHeight*2) , 2 );
    }else{
      CenterPrint4x5Text( "Try the real Game for a harder challenge" , (render_info.window_size.cy >> 1 ) + (FontHeight*2) , 2 );
    }
  }
  
  CenterPrint4x5Text( "Press Space to continue" , render_info.window_size.cy - (FontHeight*2) , 2 );
//  Our_CalculateFrameRate();


  return TRUE;
}

// ModeScaleX use to have hard coded spacing value and such for fonts etc...
// now we just use index 0 and set it dynamically to whatever screen size we are at

void InitModeCase(void)
{
  ModeScaleX = (float)render_info.window_size.cx / 320.0F;
  ModeScaleY = (float)render_info.window_size.cy / 200.0F;
}

/*===================================================================
  Procedure :   Get Bit Shift required to make first bit of
        :   mask bit 0
  Input   :   int32 Mask
  Output    :   int16 Shift Value
===================================================================*/
int16 GetBitShift( int32 Mask )
{
  int16 Count;

  if( Mask )
  {
    Count = 0;

    while( !( Mask & 1 ) )
    {
      Count++;
      Mask >>= 1;
    }

    return( Count );
  }

  return( 0 );
}


/*===================================================================
  Procedure :   Render Snapshot
  Input   :   Nothing
  Output    :   Nothing
===================================================================*/
void RenderSnapshot( void )
{
#if 0 // bjd - CHECK
//  LPDIRECT3DDEVICE lpDev = render_info.lpD3DDevice;
    render_viewport_t View = render_info.D3DViewport;

//bjd  lpDev->lpVtbl->BeginScene(lpDev);

  CurrentCamera.enable = 1;
  CurrentCamera.UseLowestLOD = TRUE;
  CurrentCamera.GroupImIn = Ships[Current_Camera_View].Object.Group;  
  CurrentCamera.Mat = Ships[Current_Camera_View].Object.FinalMat; 
  CurrentCamera.InvMat = Ships[Current_Camera_View].Object.FinalInvMat; 
  CurrentCamera.Pos = Ships[Current_Camera_View].Object.Pos;  
  CurrentCamera.Viewport = viewport;  
  CurrentCamera.Proj = proj;  
  
  CurrentCamera.Viewport.X = 0;
  CurrentCamera.Viewport.Y = 0;
  CurrentCamera.Viewport.Width = 128;
  CurrentCamera.Viewport.Height = 128;
  CurrentCamera.Viewport.ScaleX = CurrentCamera.Viewport.dwWidth / (float)2.0;
  CurrentCamera.Viewport.ScaleY = CurrentCamera.Viewport.dwHeight / (float)2.0;

/* bjd
  CurrentCamera.Viewport.dvMaxX = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwWidth),
                     RENDERVAL(2 * CurrentCamera.Viewport.dvScaleX));
  CurrentCamera.Viewport.dvMaxY = (float)D3DDivide(RENDERVAL(CurrentCamera.Viewport.dwHeight),
                     RENDERVAL(2 * CurrentCamera.Viewport.dvScaleY));
*/

  CurrentCamera.UseLowestLOD = TRUE;
  if( RenderCurrentCamera() != TRUE ) // bjd
    return;
#endif
//bjd  lpDev->lpVtbl->EndScene(lpDev);
}

/*===================================================================
  Procedure :   Save PPM Bitmap
  Input   :   int8  * Filename
        :   int8  * Screen Pointer
        :   uint32    Width
        :   uint32    Height
        :   uint32    Bytes Per Pixel
        :   uint32    LineSize
        :   uint32    RedMask
        :   uint32    GreenMask
        :   uint32    BlueMask
        :   uint32    x1, y1, x2 ,y2 ( Portion of Screen )
  Output    :   BOOL    True/False
===================================================================*/
BOOL SavePPM( uint8 * Filename, uint8 * ScreenPtr, uint32 Width, uint32 Height, uint32 BytesPerPixel,
        uint32 LineSize, uint32 RedMask, uint32 GreenMask, uint32 BlueMask,
        uint32 x1, uint32 y1, uint32 x2, uint32 y2 )
{
  uint32    XCount, YCount;
  uint32    RedMul, GreenMul, BlueMul;
  uint8   Red, Green, Blue;
  int16   RedShift, GreenShift, BlueShift;
  uint8 * LinePtr;
  int8    Header[ 256 ];
  FILE  * fp;
  int16   i;
  uint32    Val;
  uint32    Xoff, Yoff;
  uint32    Xsize, Ysize;

  if( ( x1 == x2 ) && ( y1 == y2 ) )
  {
    x1 = 0;
    y1 = 0;
    x2 = Width;
    y2 = Height;
  }

  Xoff = x1;
  Yoff = y1;
  Xsize = ( x2 - x1 );
  Ysize = ( y2 - y1 );

  fp = file_open( Filename, "wb" );

  if( fp != NULL )
  {
    sprintf( &Header[ 0 ], "P6%c#Forsaken (c)1998%c%d %d%c255%c", 10, 10, (int)Xsize, (int)Ysize, 10, 10 );

    i = 0;
    while( Header[ i ] )
    {
      fwrite( &Header[ i ], sizeof( int8 ), 1, fp );
      i++;
    }

    RedShift = GetBitShift( RedMask );
    GreenShift = GetBitShift( GreenMask );
    BlueShift = GetBitShift( BlueMask );

    RedMul = ( 256 / ( ( RedMask >> RedShift ) + 1 ) );
    GreenMul = ( 256 / ( ( GreenMask >> GreenShift ) + 1 ) );
    BlueMul = ( 256 / ( ( BlueMask >> BlueShift ) + 1 ) );

    ScreenPtr += ( Yoff * LineSize );

    for( YCount = 0; YCount < Ysize; YCount++ )
    {
      LinePtr = ( ScreenPtr + Xoff );

      for( XCount = 0; XCount < Xsize; XCount++ )
      {
        Val = 0;

        switch( BytesPerPixel )
        {
          case 1:
            Val = (uint32) LinePtr[0];
            break;
          case 2:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 );
            break;
          case 3:
            Val = (uint32) ( (uint32) LinePtr[0] ) |
                   ( ( (uint32) LinePtr[1] ) << 8 ) |
                   ( ( (uint32) LinePtr[2] ) << 16 );
            break;
        }
        LinePtr += BytesPerPixel;

        Red   = (uint8) ( ( ( Val & RedMask ) >> RedShift ) * RedMul );
        Green = (uint8) ( ( ( Val & GreenMask ) >> GreenShift ) * GreenMul );
        Blue  = (uint8) ( ( ( Val & BlueMask ) >> BlueShift ) * BlueMul );

        fwrite( &Red, sizeof( Red ), 1, fp );
        fwrite( &Green, sizeof( Green ), 1, fp );
        fwrite( &Blue, sizeof( Blue ), 1, fp );
      }

      ScreenPtr += LineSize;
    }

    fclose( fp );
  }
  else
  {
    DebugPrintf( "Failed to create %s\n", Filename );
    return( FALSE );
  }

  return( TRUE );
}

/*===================================================================
  Procedure :   Save SnapShot screen for saved game
  Input   :   int8  * Filename
  Output    :   BOOL    True/False
===================================================================*/
BOOL SaveSnapShot( int8 * Filename )
{
	return TRUE;
#if 0 // bjd
  HRESULT     hr;
  DDSURFACEDESC SurfaceDesc;

  FreeAllLastAFrameScrPolys();
  RenderSnapshot();

  memset( &SurfaceDesc, 0, sizeof( SurfaceDesc ) );
  SurfaceDesc.dwSize = sizeof( SurfaceDesc );

  hr = render_info.lpBackBuffer->lpVtbl->Lock( render_info.lpBackBuffer, NULL, &SurfaceDesc,
        DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY, NULL );

  if ( hr != DD_OK )
  {
    switch( hr )
    {
      case DDERR_INVALIDOBJECT:
        DebugPrintf( "Error Locking Surface ( Invalid Object )\n" );
        break;

      case DDERR_INVALIDPARAMS:
        DebugPrintf( "Error Locking Surface ( Invalid Params )\n" );
        break;

      case DDERR_OUTOFMEMORY:
        DebugPrintf( "Error Locking Surface ( Out of Memory )\n" );
        break;

      case DDERR_SURFACEBUSY:
        DebugPrintf( "Error Locking Surface ( Surface Busy )\n" );
        break;

      case DDERR_SURFACELOST:
        DebugPrintf( "Error Locking Surface ( Surface Lost )\n" );
        break;

      case DDERR_WASSTILLDRAWING:
        DebugPrintf( "Error Locking Surface ( Was Still Drawing )\n" );
        break;
    }
    return( FALSE );
  }

  SavePPM( Filename, SurfaceDesc.lpSurface, SurfaceDesc.dwWidth, SurfaceDesc.dwHeight,
        ( ( SurfaceDesc.ddpfPixelFormat.dwRGBBitCount + 7 )  / 8 ),
        SurfaceDesc.lPitch, SurfaceDesc.ddpfPixelFormat.dwRBitMask, SurfaceDesc.ddpfPixelFormat.dwGBitMask,
        SurfaceDesc.ddpfPixelFormat.dwBBitMask, 0, 0, 128, 128 );

  hr = render_info.lpBackBuffer->lpVtbl->Unlock( render_info.lpBackBuffer, NULL );
  if ( hr != DD_OK )
  {
    DebugPrintf( "Error Unlocking Surface\n" );
    return( FALSE );
  }

  if( CurrentMenu ) MenuDraw( CurrentMenu );
  MainGame( /*render_info.lpD3DDevice,*/ render_info.lpD3DViewport ); // bjd

  return( TRUE );
#endif
}

// if we met the max kills limit then set flag to change level
void CheckMetKillLimit()
{
	BOOL	KillsReached = FALSE;
	BOOL teamOK[MAX_TEAMS];
	BOOL teamleft[MAX_TEAMS];

	// i'm the host so i should check if we have met the level kill threshold if there is one
	if(IsHost && MaxKills && (MyGameStatus == STATUS_Normal) )
	{
		// not team game
		if( !TeamGame )
		{
			// for every player
			for ( i = 0; i < MAX_PLAYERS; i++ )
			{
				// whose status is normal
				if( (GameStatus[GetPlayerByRank(i)] == STATUS_Normal) )
				{
					// if they scored more than the kills threshold then flag we finished it
					if( GetScoreStats(GetPlayerByRank(i)) >= MaxKills && LevelNum != -1 )
						KillsReached = TRUE;
				}
			}
		}
		// team game
		else
		{
			// identify which teams are active
			for (i = 0; i < MAX_TEAMS; i++)
			{	
				teamOK[i] = FALSE;
				teamleft[i] = TRUE;
			}

			// if any of the active teams scored more than the kills threshold then flag we finished it
			for( i = 0 ; i < MAX_TEAMS ; i++ )
			{
				if( GetTeamScore(i) >= MaxKills && !teamleft[i] && teamOK[i])
					KillsReached = TRUE;
			}
		}

		// kill threshold reached so change level
		if( KillsReached )
		{
			LevelNum = -1;
			NewLevelNum++;
			if( NewLevelNum >= NumLevels ) NewLevelNum = 0;
			NetworkGameUpdate();
		}
	}

}

