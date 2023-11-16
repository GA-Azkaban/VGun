#include "ASEfile.h"

#define	NUMBER_OF_RESERVEDWORD				233
LPSTR Token[256] =
{
    (LPSTR)"*3DSMAX_ASCIIEXPORT"		/*  0*/
,  (LPSTR)"*COMMENT"					/*  1*/
,  (LPSTR)"*SCENE"						/*  2*/
,  (LPSTR)"*MATERIAL_LIST"				/*  3*/
,  (LPSTR)"*MATERIAL"					/*  4*/
,  (LPSTR)"*LIGHTOBJECT"				/*  5*/
,  (LPSTR)"*GEOMOBJECT"				/*  6*/
,  (LPSTR)"*NODE_NAME"					/*  7*/
,  (LPSTR)"*NODE_TM"					/*  8*/
,  (LPSTR)"*MESH"						/*  9*/
,  (LPSTR)"*MESH_VERTEX"				/* 10*/
,  (LPSTR)"*MESH_FACE_LIST"			/* 11*/
,  (LPSTR)"*MESH_FACE"					/* 12*/
,  (LPSTR)"*MESH_NUMTVERTEX"			/* 13*/
,  (LPSTR)"*MESH_NUMCVERTEX"			/* 14*/
,  (LPSTR)"*MESH_NORMALS"				/* 15*/
,  (LPSTR)"*MESH_FACENORMAL"			/* 16*/
,  (LPSTR)"*MESH_VERTEXNORMAL"			/* 17*/
,  (LPSTR)"*HELPEROBJECT"				/* 18*/
,  (LPSTR)"*NODE_PARENT"				/* 19*/
,  (LPSTR)"*HELPER_CLASS"				/* 20*/
,  (LPSTR)"*INHERIT_POS"				/* 21*/
,  (LPSTR)"*INHERIT_ROT"				/* 22*/
,  (LPSTR)"*INHERIT_SCL"				/* 23*/
,  (LPSTR)"*TM_ROW0"					/* 24*/
,  (LPSTR)"*TM_ROW1"					/* 25*/
,  (LPSTR)"*TM_ROW2"					/* 26*/
,  (LPSTR)"*TM_ROW3"					/* 27*/
,  (LPSTR)"*TM_POS"					/* 28*/
,  (LPSTR)"*TM_ROTAXIS"				/* 29*/
,  (LPSTR)"*TM_ROTANGLE"				/* 30*/
,  (LPSTR)"*TM_SCALE"					/* 31*/
,  (LPSTR)"*TM_SCALEAXIS"				/* 32*/
,  (LPSTR)"*TM_SCALEAXISANG"			/* 33*/
,  (LPSTR)"*BOUNDINGBOX_MIN"			/* 34*/
,  (LPSTR)"*BOUNDINGBOX_MAX"			/* 35*/
,  (LPSTR)"*TIMEVALUE"					/* 36*/
,  (LPSTR)"*MESH_NUMVERTEX"			/* 37*/
,  (LPSTR)"*MESH_NUMFACES"				/* 38*/
,  (LPSTR)"*GROUP"						/* 39*/
,  (LPSTR)"*MESH_TVERT"				/* 40*/
,  (LPSTR)"*MESH_TFACELIST"			/* 41*/
,  (LPSTR)"*MESH_TVERTLIST"			/* 42*/
,  (LPSTR)"*PROP_MOTIONBLUR"			/* 43*/
,  (LPSTR)"*PROP_CASTSHADOW"			/* 44*/
,  (LPSTR)"*PROP_RECVSHADOW"			/* 45*/
,  (LPSTR)"*MATERIAL_REF"				/* 46*/
,  (LPSTR)"*NODE_PARENT"				/* 47*/
,  (LPSTR)"*SCENE_FILENAME"			/* 48*/
,  (LPSTR)"*SCENE_FIRSTFRAME"			/* 49*/
,  (LPSTR)"*SCENE_LASTFRAME"			/* 50*/
,  (LPSTR)"*SCENE_FRAMESPEED"			/* 51*/
,  (LPSTR)"*SCENE_TICKSPERFRAME"		/* 52*/
,  (LPSTR)"*SCENE_ENVMAP"				/* 53*/
,  (LPSTR)"*MAP_NAME"					/* 54*/
,  (LPSTR)"*MAP_CLASS"					/* 55*/
,  (LPSTR)"*MAP_SUBNO"					/* 56*/
,  (LPSTR)"*MAP_AMOUNT"				/* 57*/
,  (LPSTR)"*SCENE_AMBIENT_STATIC"		/* 58*/
,  (LPSTR)"*MATERIAL_COUNT"			/* 59*/
,  (LPSTR)"*MATERIAL_NAME"				/* 60*/
,  (LPSTR)"*MATERIAL_CLASS"			/* 61*/
,  (LPSTR)"*MATERIAL_AMBIENT"			/* 62*/
,  (LPSTR)"*MATERIAL_DIFFUSE"			/* 63*/
,  (LPSTR)"*MATERIAL_SPECULAR"			/* 64*/
,  (LPSTR)"*MATERIAL_SHINE"			/* 65*/
,  (LPSTR)"*MATERIAL_SHINESTRENGTH"	/* 66*/
,  (LPSTR)"*MATERIAL_TRANSPARENCY"		/* 67*/
,  (LPSTR)"*MATERIAL_WIRESIZE"			/* 68*/
,  (LPSTR)"*MATERIAL_SHADING"			/* 69*/
,  (LPSTR)"*MATERIAL_XP_FALLOFF"		/* 70*/
,  (LPSTR)"*MATERIAL_SELFILLUM"		/* 71*/
,  (LPSTR)"*MATERIAL_TWOSIDED"			/* 72*/
,  (LPSTR)"*MATERIAL_FALLOFF"			/* 73*/
,  (LPSTR)"*MATERIAL_XP_TYPE"			/* 74*/
,  (LPSTR)"*MAP_DIFFUSE"				/* 75*/
,  (LPSTR)"*MESH_VERTEX_LIST"			/* 76*/
,  (LPSTR)"*MESH_NUMTVFACES"			/* 77*/
,  (LPSTR)"*MESH_CVERTLIST"			/* 78*/
,  (LPSTR)"*LIGHT_TYPE"				/* 79*/
,  (LPSTR)"*LIGHT_SHADOWS"				/* 80*/
,  (LPSTR)"*LIGHT_USELIGHT"			/* 81*/
,  (LPSTR)"*LIGHT_SPOTSHAPE"			/* 82*/
,  (LPSTR)"*LIGHT_USEGLOBAL"			/* 83*/
,  (LPSTR)"*LIGHT_ABSMAPBIAS"			/* 84*/
,  (LPSTR)"*LIGHT_OVERSHOOT"			/* 85*/
,  (LPSTR)"*LIGHT_SETTINGS"			/* 86*/
,  (LPSTR)"*LIGHT_COLOR"				/* 87*/
,  (LPSTR)"*LIGHT_INTENS"				/* 88*/
,  (LPSTR)"*LIGHT_ASPECT"				/* 89*/
,  (LPSTR)"*LIGHT_TDIST"				/* 90*/
,  (LPSTR)"*LIGHT_MAPBIAS"				/* 91*/
,  (LPSTR)"*LIGHT_MAPRANGE"			/* 92*/
,  (LPSTR)"*LIGHT_MAPSIZE"				/* 93*/
,  (LPSTR)"*LIGHT_RAYBIAS"				/* 94*/
,  (LPSTR) "*MESH_SMOOTHING"			/* 95*/
,  (LPSTR)"*MESH_MTLID"				/* 96*/
,  (LPSTR)"*WIREFRAME_COLOR"			/* 97*/
,  (LPSTR) "*TM_ANIMATION"				/* 98*/
,  (LPSTR) "*CONTROL_POS_TRACK"		/* 99*/
,  (LPSTR) "*CONTROL_ROT_TRACK"		/*100*/
,  (LPSTR) "*CONTROL_POS_SAMPLE"		/*101*/
,  (LPSTR) "*CONTROL_ROT_SAMPLE"		/*102*/
,  (LPSTR) "*CAMERAOBJECT"				/*103*/
,  (LPSTR) "*CAMERA_TYPE"				/*104*/
,  (LPSTR) "*CAMERA_SETTINGS"			/*105*/
,  (LPSTR) "*CAMERA_NEAR"				/*106*/
,  (LPSTR) "*CAMERA_FAR"				/*107*/
,  (LPSTR) "*CAMERA_FOV"				/*108*/
,  (LPSTR) "*CAMERA_TDIST"				/*109*/
,  (LPSTR) "*NUMSUBMTLS"				/*110*/
,  (LPSTR) "*SUBMATERIAL"				/*111*/
,  (LPSTR) "*MAP_SPECULAR"				/*112*/
,  (LPSTR) "*MAP_SHINE"				/*113*/
,  (LPSTR) "*MAP_GENERIC"				/*114*/
,  (LPSTR) "*BITMAP"					/*115*/
,  (LPSTR) "*MAP_TYPE"					/*116*/
,  (LPSTR) "*UVW_U_OFFSET"				/*117*/
,  (LPSTR) "*UVW_V_OFFSET"				/*118*/
,  (LPSTR) "*UVW_U_TILING"				/*119*/
,  (LPSTR) "*UVW_V_TILING"				/*120*/
,  (LPSTR) "*UVW_ANGLE"				/*121*/
,  (LPSTR) "*UVW_BLUR"					/*122*/
,  (LPSTR) "*UVW_BLUR_OFFSET"			/*123*/
,  (LPSTR) "*UVW_NOUSE_AMT"			/*124*/
,  (LPSTR) "*UVW_NOISE_SIZE"			/*125*/
,  (LPSTR) "*UVW_NOISE_LEVEL"			/*126*/
,  (LPSTR) "*UVW_NOISE_PHASE"			/*127*/
,  (LPSTR) "*BITMAP_FILTER"			/*128*/
,  (LPSTR)"*MESH_MAPPINGCHANNEL"		/*129*/
,  (LPSTR)"*MESH_TFACE"				/*130*/
,  (LPSTR)"*CONTROL_POS_BEZIER"		/*131*/
,  (LPSTR)"*CONTROL_BEZIER_POS_KEY"	/*132*/
,  (LPSTR)"*CONTROL_ROT_TCB"			/*133*/
,  (LPSTR)"*CONTROL_TCB_ROT_KEY"		/*134*/
,  (LPSTR)"*MAP_OPACITY"				/*135*/
,  (LPSTR)"*MATERIAL_FACEMAP"			/*136*/
,  (LPSTR) "*MESH_NUMSKINWEIGHT"		/*137*/
,  (LPSTR) "*MESH_NUMBONE"				/*138*/
,  (LPSTR) "*BONE_LIST"				/*139*/
,  (LPSTR) "*BONE_NAME"				/*140*/
,  (LPSTR) "*MESH_WVERTEXS"			/*141*/
,  (LPSTR) "*MESH_WEIGHT"				/*142*/
,  (LPSTR) "*BONE_BLENGING_WEIGHT"		/*143*/
,  (LPSTR) "*MAP_REFLECT"				/*144*/
,  (LPSTR) "*MAP_REFRACT"				/*145*/
,  (LPSTR) "*MAP_BUMP"					/*146*/
,  (LPSTR) "*SCENE_BACKGROUND_STATIC"	/*147*/
,  (LPSTR) "*MAP_SELFILLUM"			/*148*/
,  (LPSTR) "*LIGHT_EXCLUDELIST"		/*149*/
,  (LPSTR) "*LIGHT_NUMEXCLUDED"		/*150*/
,  (LPSTR) "*LIGHT_EXCLUDED_INCLUDE"	/*151*/
,  (LPSTR) "*LIGHT_EXCLUDED_AFFECT_ILLUM"	/*152*/
,  (LPSTR) "*LIGHT_EXCLUDED_AFFECT_SHADOW"	/*153*/
,  (LPSTR) "*LIGHT_EXCLUDED"			/*154*/
,  (LPSTR) "*LIGHT_HOTSPOT"			/*155*/
,  (LPSTR) "*LIGHT_FALLOFF"			/*156*/
,  (LPSTR)"*MESH_FACEMAPLIST"			/*157*/
,  (LPSTR)"*MESH_FACEMAP"				/*158*/
,  (LPSTR)"*MESH_FACEMAPVERT"			/*159*/
,  (LPSTR) "*BITMAP_INVERT"			/*160*/
,  (LPSTR) "*SHAPEOBJECT"				/*161*/
,  (LPSTR) "*SHAPE_LINECOUNT"			/*162*/
,  (LPSTR) "*SHAPE_LINE"				/*163*/
,  (LPSTR) "*SHAPE_VERTEXCOUNT"		/*164*/
,  (LPSTR) "*SHAPE_VERTEX_KNOT"		/*165*/
,  (LPSTR) "*SHAPE_VERTEX_INTERP"		/*166*/
,  (LPSTR) "*SHAPE_CLOSED"				/*167*/
,  (LPSTR) "*IK_JOINT"					/*168*/
,  (LPSTR) "*IK_TYPE"					/*169*/
,  (LPSTR) "*IK_DOF"					/*170*/
,  (LPSTR) "*IK_XACTIVE"				/*171*/
,  (LPSTR) "*IK_YACTIVE"				/*172*/
,  (LPSTR) "*IK_ZACTIVE"				/*173*/
,  (LPSTR) "*IK_XLIMITED"				/*174*/
,  (LPSTR) "*IK_YLIMITED"				/*175*/
,  (LPSTR) "*IK_ZLIMITED"				/*176*/
,  (LPSTR) "*IK_XEASE"					/*177*/
,  (LPSTR) "*IK_YEASE"					/*178*/
,  (LPSTR) "*IK_ZEASE"					/*179*/
,  (LPSTR) "*IK_LIMITEXACT"			/*180*/
,  (LPSTR) "*IK_JOINTINFO"				/*181*/
,  (LPSTR) "*LIGHT_ATTNSTART"			/*182*/
,  (LPSTR) "*LIGHT_ATTNEND"			/*183*/
,  (LPSTR) "*MAP_AMBIENT"				/*184*/
,  (LPSTR) "*MESH_VERTCOL"				/*185*/
,  (LPSTR) "*MESH_NUMCVFACES"			/*186*/
,  (LPSTR) "*MESH_CFACELIST"			/*187*/
,  (LPSTR) "*MESH_CFACE"				/*188*/

,  (LPSTR) "*MAP_SHINESTRENGTH"		/*189*/
,  (LPSTR) "*MAP_FILTERCOLOR"			/*190*/

,  (LPSTR) "*NODE_VISIBILITY_TRACK"	/*191*/
,  (LPSTR) "*CONTROL_FLOAT_SAMPLE"		/*192*/
,  (LPSTR) "*CONTROL_FLOAT_KEY"		/*193*/

,  (LPSTR) "*BONE_PROPERTY"			/*194*/
,  (LPSTR) "*BONE"						/*195*/
,  (LPSTR) "*SKIN_INITTM"				/*196*/
,  (LPSTR) "*CONTROL_SCALE_TRACK"		/*197*/
,  (LPSTR) "*CONTROL_SCALE_SAMPLE"		/*198*/
,  (LPSTR) "*SCENE_AMBIENT_ANIM"		/*199*/
,  (LPSTR) "*CONTROL_POINT3_KEY"		/*200*/
,  (LPSTR) "*CONTROL_TCB_POINT3_KEY"	/*201*/
,  (LPSTR) "*CONTROL_TCB_FLOAT_KEY"	/*202*/
,  (LPSTR) "*CONTROL_TCB_POS_KEY"		/*203*/
,  (LPSTR) "*CONTROL_TCB_SCALE_KEY"	/*204*/
,  (LPSTR) "*CONTROL_BEZIER_FLOAT_KEY"	/*205*/
,  (LPSTR) "*CONTROL_BEZIER_POINT3_KEY"/*206*/
,  (LPSTR) "*CONTROL_BEZIER_SCALE_KEY"	/*207*/
,  (LPSTR) "*CONTROL_POS_LINEAR"		/*208*/
,  (LPSTR) "*CONTROL_POS_TCB"			/*209*/
,  (LPSTR) "*CONTROL_ROT_LINEAR"		/*210*/
,  (LPSTR) "*CONTROL_ROT_BEZIER"		/*211*/
,  (LPSTR) "*CONTROL_SCALE_LINEAR"		/*212*/
,  (LPSTR) "*CONTROL_SCALE_TCB"		/*213*/
,  (LPSTR) "*CONTROL_SCALE_BEZIER"		/*214*/

,  (LPSTR) "*CONTROL_POS_KEY"			/*215*/
,  (LPSTR) "*CONTROL_ROT_KEY" 			/*216*/
,  (LPSTR) "*CONTROL_SCALE_KEY"		/*217*/

,  (LPSTR) "*CONTROL_POINT3_TCB"		/*218*/
,  (LPSTR) "*CONTROL_POINT3_BEZIER"	/*219*/
,  (LPSTR) "*CONTROL_COLOR_BEZIER"		/*220*/
,  (LPSTR) "*CONTROL_POINT3_SAMPLE"	/*221*/

,  (LPSTR) "*CONTROL_FLOAT_TCB"		/*222*/
,  (LPSTR) "*CONTROL_FLOAT_BEZIER"		/*223*/
,  (LPSTR) "*CONTROL_FLOAT_LINEAR"		/*224*/

,  (LPSTR) "*CAMERA_ANIMATION"			/*225*/
,  (LPSTR) "*LIGHT_ANIMATION"			/*226*/

,  (LPSTR) "*MATERIAL_WIREUNITS"		/*227*/

,  (LPSTR) "*SCENE_MESHFRAMESTEP"		/*228*/
,  (LPSTR) "*SCENE_KEYFRAMESTEP"		/*229*/

,  (LPSTR) "*PROP_HIDDEN"				/*230*/
,  (LPSTR) "*PROP_NORENDERABLE"		/*231*/
,  (LPSTR) "*PROP_BONE"				/*232*/
, };


ASE::ASELexer::ASELexer(LPSTR p_FileName, DWORD p_SizeOfBuffer) : Lexer(p_FileName, p_SizeOfBuffer)
{
}

ASE::ASELexer::~ASELexer()
{
}

//Lexical Analysis
LONG ASE::ASELexer::GetToken(LPSTR p_TokenString)
{
    // 1) 앞쪽 쓸데없는 문자찾기
    //    앞쪽에 공백이나 Tab이나 개행이나 캐리지리턴 같은 문자열이 있으면 무시한다.
    CHAR	Char = GetNextChar();
    while (Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
    {
        Char = GetNextChar();
        if (!Char)
        {
            return	TOKEND_END;
        }
    }

    // 2) '{'인가를 확인한다.
    if (Char == '{')
    {
        p_TokenString[0] = '{';
        p_TokenString[1] = NULL;

        return	TOKEND_BLOCK_START;
    }

    // 3) '}'인가를 확인한다.
    else if (Char == '}')
    {
        p_TokenString[0] = '}';
        p_TokenString[1] = NULL;

        return	TOKEND_BLOCK_END;
    }
    // 4) '*'인가를 확인한다.
    //    만약 '*'로 시작하는 것은 Reserved word이므로 검색한다!
    else if (Char == '*')
    {
        LONG	p_StringPoint = 0;
        while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}')
        {
            p_TokenString[p_StringPoint] = Char;

            Char = GetNextChar();
            if (!Char)
            {
                return	TOKEND_END;
            }
            p_StringPoint++;
        }
        p_TokenString[p_StringPoint] = NULL;

        for (int i = 0; i < NUMBER_OF_RESERVEDWORD; i++)
        {
            if (!strcmp(p_TokenString, Token[i]))
            {
                // 맞는 Token을 찾았다! 그 번호를 return시켜준다.
                return	i;
            }
        }

        // 맞는 것은 전혀 찾지 못했다. 정의되지 않은 TOKEN이라고 돌려준다.
        return	TOKEND_NOTDEFINED;
    }
    // 5) 숫자인지를 확인한다.
    //    만약에 '0'에서 '9'사이이거나 '-'부하고 붙어 있으면 숫자라고 판단을 한다.
    else if (Char >= '0' && Char <= '9' || Char == '-')
    {
        LONG	p_StringPoint = 0;
        while ((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
        {
            p_TokenString[p_StringPoint] = Char;

            Char = GetNextChar();
            if (!Char)
            {
                return	TOKEND_END;
            }
            p_StringPoint++;
        }
        p_TokenString[p_StringPoint] = NULL;


        return	TOKEND_NUMBER;
    }
    // 6) String인지를 검사한다.
    // '"' 따음표로 시작하면 이것은 String인다!
    else if (Char == '"')
    {
        Char = GetNextChar();
        if (!Char)
        {
            return	TOKEND_END;
        }

        LONG	p_StringPoint = 0;
        while (Char != '"')
        {
            p_TokenString[p_StringPoint] = Char;

            Char = GetNextChar();
            if (!Char)
            {
                return	TOKEND_END;
            }
            p_StringPoint++;
        }
        p_TokenString[p_StringPoint] = NULL;

        return	TOKEND_STRING;
    }
    // 7) 지금까지 못찾았음 이건 허당이다~ 다음을 기약하자~
    else
    {
        // 첫글짜는 char이 보관하고 있으니 이것을 일단 p_TokenStirng에 넣고 다음 글자를 읽어 들인다.
        p_TokenString[0] = Char;
        Char = GetNextChar();
        if (!Char)
        {
            return	TOKEND_END;
        }

        // 한 단어를 p_TokeString에다가 복사를 한다.
        LONG	p_StringPoint = 1;
        while (Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n')
        {
            p_TokenString[p_StringPoint] = Char;

            Char = GetNextChar();
            if (!Char)
            {
                return	TOKEND_END;
            }
            p_StringPoint++;
        }
        p_TokenString[p_StringPoint] = NULL;

        // 일반적인 Identifier라고 return한다.(Idendifier는 변수이름이라든지 뭐 그런거..)
        return	TOKEND_IDENTIFIER;
    }

    //p_TokenString[0]	 = NULL;

    //TRACE("Have no match token : %s\n", p_TokenString);
    //return TOKEND_NOTDEFINED;
}