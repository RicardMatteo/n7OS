#include <n7OS/console.h>
#include <n7OS/time.h>

uint16_t *scr_tab;
// cursor position
// we start at the second line to leave the first line for the time
int cursor_pos = VGA_WIDTH;

void init_console()
{
    scr_tab = (uint16_t *)SCREEN_ADDR;

    console_full_clear();
    // write n7OS on the top left corner
    scr_tab[0] = HEADER_COLOR << 8 | 'n';
    scr_tab[1] = HEADER_COLOR << 8 | '7';
    scr_tab[2] = HEADER_COLOR << 8 | 'O';
    scr_tab[3] = HEADER_COLOR << 8 | 'S';
}

void console_scroll()
{
    for (int i = VGA_WIDTH; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
    {
        scr_tab[i] = scr_tab[i + VGA_WIDTH];
    }
    for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        scr_tab[i] = CHAR_COLOR << 8 | ' ';
    }
    cursor_pos = VGA_WIDTH * (VGA_HEIGHT - 1);
}

void console_putchar(const char c)
{
    if (c > 31 && c < 127)
    {
        scr_tab[cursor_pos] = CHAR_COLOR << 8 | c;
        cursor_pos++;
    }
    else if (c == '\n')
    {
        // newline
        cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
    }
    else if (c == '\b')
    {
        // erase
        scr_tab[cursor_pos - 1] = CHAR_COLOR << 8 | ' ';
        // go backward
        // check if we are not at the beginning
        if (cursor_pos > VGA_WIDTH)
        {
            cursor_pos--;
        }
    }
    else if (c == '\t')
    {
        // tab
        cursor_pos = cursor_pos + 8;
    }
    else if (c == '\r')
    {
        // carriage return
        cursor_pos = (cursor_pos / VGA_WIDTH) * VGA_WIDTH;
    }
    else if (c == '\f')
    {
        // clear screen and move cursor to top left
        console_clear();
    }
    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT)
    {
        console_scroll();
    }
    console_cursor(cursor_pos);
}

void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i++)
    {
        console_putchar(s[i]);
    }
}

void console_clear()
{
    for (int i = VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        scr_tab[i] = CHAR_COLOR << 8 | ' ';
    }
    cursor_pos = VGA_WIDTH;
}

void console_full_clear()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        scr_tab[i] = CHAR_COLOR << 8 | ' ';
    }
    cursor_pos = VGA_WIDTH;
}

void console_cursor(int pos)
{
    outb(CMD_LOW, PORT_CMD);
    outb(pos & 0xFF, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb((pos >> 8) & 0xFF, PORT_DATA);
}

// put the time on the top right corner of the screen
void console_put_time(time_t time)
{
    int pos = VGA_WIDTH - 8;
    scr_tab[pos] = HEADER_COLOR << 8 | ('0' + time.hours / 10);
    scr_tab[pos + 1] = HEADER_COLOR << 8 | ('0' + time.hours % 10);
    scr_tab[pos + 2] = HEADER_COLOR << 8 | ':';
    scr_tab[pos + 3] = HEADER_COLOR << 8 | ('0' + time.minutes / 10);
    scr_tab[pos + 4] = HEADER_COLOR << 8 | ('0' + time.minutes % 10);
    scr_tab[pos + 5] = HEADER_COLOR << 8 | ':';
    scr_tab[pos + 6] = HEADER_COLOR << 8 | ('0' + time.seconds / 10);
    scr_tab[pos + 7] = HEADER_COLOR << 8 | ('0' + time.seconds % 10);
}

void console_put_arbitrary(int pos, char c)
{
    if (pos < VGA_WIDTH * VGA_HEIGHT)
    {
        scr_tab[pos] = HEADER_COLOR << 8 | c;
    }
    else
    {
        scr_tab[pos] = CHAR_COLOR << 8 | c;
    }
}

void console_put_arbitrary_color(int pos, char c, char color)
{
    scr_tab[pos] = color << 8 | c;
}

void console_put_score(int score)
{
    int score_len = 1;
    while (score > 1)
    {
        scr_tab[VGA_WIDTH - (8 + score_len + 5)] = CHAR_COLOR << 8 | ('0' + score % 10);
        score_len++;

        score /= 10;
    }

    int pos = VGA_WIDTH - (8 + score_len + 6 + 5);
    scr_tab[pos] = CHAR_COLOR << 8 | 'S';
    scr_tab[pos + 1] = CHAR_COLOR << 8 | 'c';
    scr_tab[pos + 2] = CHAR_COLOR << 8 | 'o';
    scr_tab[pos + 3] = CHAR_COLOR << 8 | 'r';
    scr_tab[pos + 4] = CHAR_COLOR << 8 | 'e';
    scr_tab[pos + 5] = CHAR_COLOR << 8 | ':';
}

void console_put_cursor(int pos)
{
    cursor_pos = pos;
}

void exec_line()
{
    // we need to get the last line used in the console
    int current_line = cursor_pos / VGA_WIDTH;
    int size = cursor_pos % VGA_WIDTH;
    // jump to the next line
    cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;

    // we check if this line contains a command

    // we need to get the command
    char command[VGA_WIDTH];
    int dec = 0;
    // check if the first character is a '>'
    if ((scr_tab[current_line * VGA_WIDTH] & 0xFF) == '>')
    {
        dec = 1;
        size--;
    }
    for (int j = 0; j < size; j++)
    {
        command[j] = scr_tab[current_line * VGA_WIDTH + j + dec];
    }
    /*
    while ((i < VGA_WIDTH) && ((scr_tab[current_line * VGA_WIDTH + i + dec] & 0xFF) != ' '))
    {
        command[i] = scr_tab[current_line * VGA_WIDTH + i + dec];
        // print the character
        console_putchar(command[i]);
        console_putchar(' ');
        i++;
    }
    */

    command[size] = '\0'; /*
     // we need to get the arguments
     char args[80];
     i = 0;
     while ((i < VGA_WIDTH) && (scr_tab[current_line * VGA_WIDTH + i] != CHAR_COLOR << 8 | ' '))
     {
         args[i] = scr_tab[current_line * VGA_WIDTH + i];
         i++;
     }
     args[i] = '\0';*/
    //  we need to execute the command
    int return_value;
    return_value = syscall(command, size);
    if (return_value == -1)
    {
        printf("command %s failed\n", command);
    }
    /*
    if (strcmpk(command, "clear", 5) == 0)
    {
        sys_clear();
    }
    else if (strcmpk(command, "help", 4) == 0)
    {
        console_putbytes("Available commands:\n", 20);
        console_putbytes("clear: clear the screen\n", 24);
        console_putbytes("help: display this help\n", 25);
    }
    else if (i > 1)
    {
        console_putbytes("Unknown command. Type 'help' for a list of available commands.\n", 64);
        // print the command
    }
    */
    console_putchar('>');
}

void print_help()
{
    console_putchar('\n');
    console_putbytes("Available commands:\n", 20);
    console_putbytes("clear: clear the screen\n", 24);
    console_putbytes("help: display this help\n", 25);
    console_putbytes("process: test the process management\n", 38);
    console_putbytes("snake: play the snake game\n", 28);
    console_putbytes("print_memory: print the memory\n", 32);
    console_putbytes("ps: print the processes\n", 25);
    console_putbytes("shutdown: shutdown the system\n", 31);
    console_putbytes("+ all the syscall commands\n", 28);
    console_putchar('\n');
}
