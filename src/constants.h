#pragma once

#include "ofMain.h"

class constants: public ofBaseApp
{

public:
    const static string SHADER_COMPOSER_FILE_NAME;

    const static string DEFAULT_JSON_STYLES_FILE_NAME;
    const static string CUSTOM_JSON_STYLES_FILE_NAME;

    //JSON STYLE format key names
    const static string STYLES_KEY;
        const static string VIZ_VARS_KEY;
        const static string FXS_KEY;
            const static string FX_ID_KEY;
            const static string POSITION_KEY;


    const static string STYLE_UID_KEY;
        const static string NAME_KEY;
        //const static string AUTHOR_KEY;
        //const static string DESCRIPTION_KEY;
        const static string TIMESTAMP_KEY;
        const static string EXPRESSIONS_KEY;
            const static string EXPRESSION_KEY;
            const static string EXPRESSION_ID_KEY;
};
