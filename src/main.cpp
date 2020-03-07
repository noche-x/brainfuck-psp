#include "common.h"
#include <pspkernel.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>

//This is the PSP_MODULE_HEADER
PSP_MODULE_INFO("Hello World", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

// make it easier to type
#define pspprint pspDebugScreenPrintf

int onScreenKeyboard(unsigned short *descritpion, unsigned short *outtext, int maxtextinput)
{

    //osk params
    SceUtilityOskData oskData;
    SceUtilityOskParams oskParams;

    //init osk data
    memset(&oskData, 0, sizeof(SceUtilityOskData));
    oskData.language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT; // Use system default for text input
    oskData.lines = 1;
    oskData.unk_24 = 1;
    oskData.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL; // Allow all input types
    oskData.desc = descritpion;
    //oskData.intext = intext[i];
    oskData.outtextlength = maxtextinput;
    oskData.outtextlimit = 32; // Limit input to 32 characters
    oskData.outtext = outtext;

    //init osk dialog
    memset(&oskParams, 0, sizeof(SceUtilityOskParams));
    oskParams.base.size = sizeof(oskParams);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &oskParams.base.language);
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &oskParams.base.buttonSwap);
    oskParams.base.graphicsThread = 17;
    oskParams.base.accessThread = 19;
    oskParams.base.fontThread = 18;
    oskParams.base.soundThread = 16;
    oskParams.datacount = 1;
    oskParams.data = &oskData;

    sceUtilityOskInitStart(&oskParams);

    bool done = true;

    while (done)
    {
        switch (sceUtilityOskGetStatus())
        {
        case PSP_UTILITY_DIALOG_INIT:
            break;

        case PSP_UTILITY_DIALOG_VISIBLE:
            sceUtilityOskUpdate(1);
            break;

        case PSP_UTILITY_DIALOG_QUIT:
            sceUtilityOskShutdownStart();
            break;

        case PSP_UTILITY_DIALOG_FINISHED:
            break;

        case PSP_UTILITY_DIALOG_NONE:
            done = false;

        default:
            break;
        }
    }

    if (oskData.result == PSP_UTILITY_OSK_RESULT_CANCELLED)
        return -1;

    return 0;
}

int main()
{
    pspDebugScreenInit();
    setupExitCallback();

    char tape[30000] = {0};
    for (int i = 0; i < 30000; i++)
        tape[i] = 0;
    char *ptr = tape;

    sceKernelDcacheWritebackInvalidateAll();
    // TODO show open from file or code directly menu
    // TODO show file manager
    std::cout << "[+] hi psp link!" << std::endl;

    int o_x = pspDebugScreenGetX(), o_y = pspDebugScreenGetX();
    pspDebugScreenSetXY(0, 33);
    pspprint("up: , down: . left: <  right: >  x: -  o: ]  /\: +  []: [  start: run");
    pspDebugScreenSetXY(o_x, o_y);

    pspprint(" _          _        _ ___  \n");
    pspprint("|_._ _.o.__|_   _|__|_(_|_) \n");
    pspprint("|_| (_||| || |_(_|< | __|   \n");
    pspprint("\n");

    // log enter the brainfuck code
    pspprint("[i] enter your brainfuck code: \n");

    // if button then command

    SceCtrlData buttonInput;

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    std::string contents;

    while (isRunning())
    {
        sceCtrlPeekBufferPositive(&buttonInput, 1);

        if (buttonInput.Buttons != 0)
        {
            if (buttonInput.Buttons & PSP_CTRL_LEFT)
            {
                contents += "<";
                pspprint("<");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_RIGHT)
            {
                contents += ">";
                pspprint(">");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_UP)
            {
                contents += ".";
                pspprint(".");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_DOWN)
            {
                contents += ",";
                pspprint(",");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_CROSS)
            {
                contents += "-";
                pspprint("-");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_TRIANGLE)
            {
                contents += "+";
                pspprint("+");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_SQUARE)
            {
                contents += "[";
                pspprint("[");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_CIRCLE)
            {
                contents += "]";
                pspprint("]");
                sceKernelDelayThread(1000000 / 7);
            }
            if (buttonInput.Buttons & PSP_CTRL_START)
            {
                sceKernelDelayThread(1000000 / 7);
                break;
            }
            if (buttonInput.Buttons & PSP_CTRL_HOME)
            {
                sceKernelExitGame();
                sceKernelDelayThread(1000000 / 7);
                break;
            }
        }
    }

    // std::cout << "getting the contents of " << argv[1] << std::endl;
    // std::ifstream in(argv[1]);
    // if (in.good())
    //     contents = std::string((std::istreambuf_iterator<char>(in)),
    //                            std::istreambuf_iterator<char>());
    // in.close();

    //contents = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
    char *input = (char *)contents.c_str();

    pspprint("\n");
    pspprint("[i] your code is: \n");
    pspprint(input);
    pspprint("\n");

    size_t i;
    size_t loop;
    char current_char;
    std::cout << "output: " << std::endl;
    std::string output;
    for (i = 0; input[i] != 0; i++)
    {
        //int x = pspDebugScreenGetX();
        //int y = pspDebugScreenGetY();
        current_char = input[i];
        switch (current_char)
        {
        case '>':
            ++ptr;
            break;
        case '<':
            --ptr;
            break;
        case '+':
            ++*ptr;
            break;
        case '-':
            --*ptr;
            break;
        case '.':
            std::cout << ptr;
            output += *ptr;
            break;
        case ',':
        {
            std::string field;
            unsigned short temp[128];
            const char *label = "test";
            unsigned short *desc = (unsigned short *)strtol(label, NULL, 0);
            //unsigned short desc[10] = { 'I','p',' ',' ',' ',' ',' ',' ', ' ', ' ' };
            std::string conv = "";
            if (onScreenKeyboard(desc, temp, 128) != -1)
            {
                for (int j = 0; temp[j]; j++)
                {
                    unsigned c = temp[j];

                    if (32 <= c && c <= 127) // print ascii only
                        conv += c;
                }

                field = conv;
            }
            *ptr = field.at(0);
            break;
        }

        case '[':
            break;
        case ']':
            if (*ptr)
            {
                loop = 1;
                while (loop > 0)
                {
                    current_char = input[--i];
                    if (current_char == '[')
                    {
                        loop--;
                    }
                    else if (current_char == ']')
                    {
                        loop++;
                    }
                }
                break;
            }
            else
            {
                break;
            }

        default:
            break;
        }
    }
    pspprint(output.c_str());
    sceKernelDelayThread(5000000);
    sceKernelExitGame();
    return 0;
}