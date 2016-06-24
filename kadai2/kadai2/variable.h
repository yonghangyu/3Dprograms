static bool isTetrahegon = true;
static bool isCube = false;
static bool isOctahegron = false;
static bool isDodecahedron = false;
static bool isIcosahedron = false;
static bool isleftKeyPressed=false;
static int  isLightOn;
static int typeSwitch;
static bool isShading = true;
static bool isLine = false;
static bool isPoint = false;
static int isShading_Line_point;
static int isObjectType;// object you want to switch among three types (lines ,faces ,points)
extern int isFlatOrSmooth;//1 is flat,0 is smooth ,flat is default

static int info_Tetra = 0;// count number to control display info of poly only once.
static int info_Cube = 0;
static int info_Octa = 0;
static int info_Dode = 0;
static int info_Icos = 0;
static int count = 0;
static int count_Tetra = 0;
static int count_Cube = 0;
static int count_Octa = 0;
static int count_Dode = 0;
static int count_Icos = 0;
extern int ColorCodingType; // 1 is valence based type,2 is GaussCurvature type,3 is MeanCurvature type;
static int curCount = 0;
static double MaxGauss , MinGauss ;
static double MaxMean , MinMean ;
 