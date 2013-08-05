/****************************************************************************
 Copyright (c) 2013      Zynga Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCFont.h"
#include "ccUTF8.h"

#include "CCFontFNT.h"
#include "CCFontFreeType.cpp"

NS_CC_BEGIN

const char * Font::_glyphASCII = "\"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþ ";

const char * Font::_glyphNEHE =  "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";


Font::Font() : _usedGlyphs(GlyphCollection::ASCII), _customGlyphs(nullptr)
{
}

const char * Font::getGlyphCollection(GlyphCollection glyphs)
{
    switch (glyphs)
    {
        case GlyphCollection::NEHE:
            return _glyphNEHE;
            break;
            
        case GlyphCollection::ASCII:
            return _glyphASCII;
            break;
            
        default:
            return 0;
            break;
    }
}

void Font::setCurrentGlyphCollection(GlyphCollection glyphs, const char *customGlyphs)
{
    if (_customGlyphs)
        delete [] _customGlyphs;
    
    switch (glyphs)
    {
        case GlyphCollection::NEHE:
            _customGlyphs = 0;
            break;
            
        case GlyphCollection::ASCII:
            _customGlyphs = 0;
            break;
            
        default:
            
            int lenght = strlen(customGlyphs);
            _customGlyphs = new char [lenght + 2];
            memcpy(_customGlyphs, customGlyphs, lenght);
            
            _customGlyphs[lenght]   = 0;
            _customGlyphs[lenght+1] = 0;
            
            break;
    }
}

const char * Font::getCurrentGlyphCollection()
{
    if (_customGlyphs)
        return _customGlyphs;
    else
    {
        return getGlyphCollection(_usedGlyphs);
    }
}


Font* Font::createWithTTF(const char* fntName, int fontSize, GlyphCollection glyphs, const char *customGlyphs)
{
    
    if( glyphs == GlyphCollection::DYNAMIC )
    {
        log("ERROR: GlyphCollection::DYNAMIC is not supported yet!");
        return nullptr;
    }
    
    // create the font
    FontFreeType *tempFont = FontFreeType::create();
    
    if (!tempFont)
        return nullptr;
    
    tempFont->setCurrentGlyphCollection(glyphs, customGlyphs);
    
    if( !tempFont->createFontObject(fntName, fontSize))
        return nullptr;
    
    return tempFont;
}

Font* Font::createWithFNT(const char* fntFilePath)
{
    CCBMFontConfiguration *newConf = FNTConfigLoadFile(fntFilePath);
    if (!newConf)
        return nullptr;
    
    // add the texture
    Texture2D *tempTexture = TextureCache::getInstance()->addImage(newConf->getAtlasName());
    if ( !tempTexture )
    {
        delete newConf;
        return nullptr;
    }
    
    FontFNT *tempFont =  FontFNT::create(newConf);
    
    if (!tempFont)
    {
        delete newConf;
        return nullptr;
    }
    
    return tempFont;
}

unsigned short int  * Font::getUTF16Text(const char *pText, int &outNumLetters)
{
    unsigned short* utf16String = cc_utf8_to_utf16(pText);
    
    if(!utf16String)
        return 0;
    
    outNumLetters = cc_wcslen(utf16String);
    return utf16String;
}

int Font::getUTF16TextLenght(unsigned short int *pText)
{
     return cc_wcslen(pText);
}

unsigned short int  * Font::trimUTF16Text(unsigned short int *pText, int newBegin, int newEnd)
{
    if ( newBegin<0 || newEnd<=0 )
        return 0;
    
    if ( newBegin>=newEnd )
        return 0;
    
    if (newEnd >= cc_wcslen(pText))
        return 0;
    
    int newLenght = newEnd - newBegin + 2;
    unsigned short* trimmedString = new unsigned short[newLenght];
    
    for(int c = 0; c < (newLenght-1); ++c)
    {
        trimmedString[c] = pText[newBegin + c];
    }
    
    // last char
    trimmedString[newLenght-1] = 0x0000;
    
    // done
    return trimmedString;
}

Rect Font::getRectForChar(unsigned short theChar)
{
    Rect temp;
    temp.size.width  = 0;
    temp.size.height = 0;
    temp.origin.x = 0;
    temp.origin.y = 0;
    
    return temp;
}

NS_CC_END


