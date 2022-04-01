#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LINE_LEN 200

bool notCommented(const char *);
void fixNumberCustomWidth(char *, int);
unsigned char hex2int(char);

const char *const MATCH_STRING = "R_key=";
const char *const inputFileStr = "in.txt";
const char *const outputFileStr = "out.txt";

int main(int argc, char **argv)
{
    bool variable_output_width = false;

    // if (argc > 1)
    // {
    //     if (!strcmp(argv[1], "-v"))
    //     {
    //         variable_output_width = true;
    //         puts("variable output address width!");
    //     }
    // }

    const int MATCH_STRING_LEN = strlen(MATCH_STRING);

    FILE *inFile = fopen(inputFileStr, "r");
    if (inFile == NULL)
    {
        printf("could not read input file \"%s\"\n", inputFileStr);
        getchar();
        return 1;
    }
    FILE *outFile = fopen(outputFileStr, "w");

    printf("input file: \"%s\"\noutput file: \"%s\"\n", inputFileStr, outputFileStr);
    // puts("start HR modbus address in HEX (default = 0x601):");
    int counting_index = 0x1001;
    char line[MAX_LINE_LEN];
    // if (scanf("%x", &counting_index) == 0)
    //     scanf("%s", line);
    // getchar();
    const int first_address = counting_index;

    while (fgets(line, sizeof line, inFile))
    {
        char *strp;
        // printf("%s",line);
        if ((strp = strstr(line, MATCH_STRING)) && notCommented(line))
        {
            // printf("math found");
            char *strp_end = strstr(line, ";");
            *strp_end = '\0';

            strp += MATCH_STRING_LEN + 2;
            // leggo nome parametro
            char param_name[50] = {'\0'};
            strcpy(param_name, strp);

            strcat(line, "_VISIBILITY;\n");
            fputs(line, outFile);

            // devo leggere indirizzo MODBUS del parametro
            bool found_address = false;
            while (!found_address)
            {
                fgets(line, sizeof line, inFile);
                if ((strp = strstr(line, "R_addr=$")) && notCommented(line))
                    found_address = true;
            }
            strp += 8; // length of "R_addr=$"
            unsigned int address = 0;
            while (isxdigit(*strp))
            {
                address <<= 4; // *16
                address += hex2int(*strp);
                strp++;
            }
            address -= 0x600-0x1000;
            // printf("address is %x", address);
            // counting_index=address;

            ////////////////
            line[0] = '\0';
            strcat(line, "R_var=MODBUS_HLDREG; R_addr=$");

            // converto counting_index to string
            char num_str[5];
            // /*int num_str_len = */ sprintf(num_str, "%X", counting_index++);
            sprintf(num_str, "%X", address);
            strcat(line, num_str);
            strcat(line, ";\n");

            fputs(line, outFile);

            /////////////////
            line[0] = '\0';
            strcat(line, "Desc=\"LIDVisibility..");
            strcat(line, param_name);
            strcat(line, "\";\n");
            fputs(line, outFile);

            line[0] = '\0';
            strcat(line, "DescEng=\"");
            strcat(line, param_name);
            strcat(line, " visibility\";\n");
            fputs(line, outFile);

            line[0] = '\0';
            strcat(line, "DescIta=\"Visibilit\xe0 ");
            strcat(line, param_name);
            strcat(line, "\";\n");
            fputs(line, outFile);

            line[0] = '\0';
            strcat(line, "<<<\n");
            // strcat(line, param_name);
            // strcat(line, "\";\n");
            fputs(line, outFile);

            // strp += MATCH_STRING_LEN;

            // int target_width = num_str_len;
            // if (!variable_output_width)
            //     target_width = 4;
            // fixNumberCustomWidth(strp, target_width);

            // for (int i = 0; i < target_width; i++)
            // {
            //     // aggiungo 0 all'inizio per avere lunghezza desiderata
            //     if (i < target_width - num_str_len)
            //         *strp++ = '0';
            //     else
            //         *strp++ = num_str[i + num_str_len - target_width];
            // }
        }
    }

    fclose(inFile);
    fclose(outFile);

    puts("DONE");
    printf("first address: $%X\n", first_address);
    printf("last address:  $%X\n", counting_index - 1);
    printf("total: %d\n", counting_index - first_address);
    // getchar();

    return 0;
}

unsigned char hex2int(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

void fixNumberCustomWidth(char *p, int n)
{
    int count = 0;
    while (isxdigit(p[count]))
        count++;

    if (count == n)
        return;

    if (count < n)
        memmove(p + n - count, p, strlen(p) + 1);
    else
        memmove(p, p + count - n, strlen(p + count - n) + 1);
}

bool notCommented(const char *inputstr)
{
    char *strp = strstr(inputstr, "(*");
    if (strp == NULL)
        return true;

    for (int i = 0; i < strp - inputstr; i++)
    {
        if (!isspace(inputstr[i]))
            return true;
    }

    return false;
}