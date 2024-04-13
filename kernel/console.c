#include <n7OS/console.h>

uint16_t *scr_tab;
// cursor position
// we start at the second line to leave the first line for the time
int cursor_pos = VGA_WIDTH;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;

    console_full_clear();
    // write n7OS on the top left corner
    scr_tab[0]= CHAR_COLOR<<8|'n';
    scr_tab[1]= CHAR_COLOR<<8|'7';
    scr_tab[2]= CHAR_COLOR<<8|'O';
    scr_tab[3]= CHAR_COLOR<<8|'S';
}

void console_scroll() {
    for (int i=VGA_WIDTH; i<VGA_WIDTH*(VGA_HEIGHT-1); i++) {
        scr_tab[i] = scr_tab[i + VGA_WIDTH];
    }
    for (int i=VGA_WIDTH*(VGA_HEIGHT-1); i<VGA_WIDTH*VGA_HEIGHT; i++) {
        scr_tab[i] = CHAR_COLOR<<8|' ';
    }
    cursor_pos= VGA_WIDTH*(VGA_HEIGHT-1);
}


void console_putchar(const char c) {
    if (c > 31 && c < 127) {
        scr_tab[cursor_pos]= CHAR_COLOR<<8|c;
        cursor_pos++;
    } else if (c == '\n') {
        // newline
        cursor_pos= (cursor_pos/VGA_WIDTH + 1) * VGA_WIDTH;
    } else if (c == '\b') {
        // go backwards
        cursor_pos--;
    } else if (c == '\t') {
        // tab
        cursor_pos = cursor_pos+8;
    } else if (c == '\r') {
        // carriage return
        cursor_pos = (cursor_pos/VGA_WIDTH) * VGA_WIDTH;
    } else if (c == '\f') {
        // clear screen and move cursor to top left
        console_clear();
    }
    if (cursor_pos >= VGA_WIDTH*VGA_HEIGHT) {
        console_scroll();
    }
    console_cursor(cursor_pos);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

void console_clear() {
    for (int i = VGA_WIDTH; i<VGA_WIDTH*VGA_HEIGHT; i++) {
        scr_tab[i]= CHAR_COLOR<<8|' ';
    }
    cursor_pos = VGA_WIDTH;
}

void console_full_clear() {
    for (int i = 0; i<VGA_WIDTH*VGA_HEIGHT; i++) {
        scr_tab[i]= CHAR_COLOR<<8|' ';
    }
    cursor_pos= VGA_WIDTH;
}

void console_cursor(int pos) {
    outb(CMD_LOW, PORT_CMD);
    outb(pos&0xFF, PORT_DATA);
    outb(CMD_HIGH, PORT_CMD);
    outb((pos>>8)&0xFF, PORT_DATA);
}

// put the time on the top right corner of the screen
void console_put_time(time_t time) {
    int pos = VGA_WIDTH - 8;
    scr_tab[pos] = CHAR_COLOR<<8|'0' + time.hours/10;
    scr_tab[pos+1] = CHAR_COLOR<<8|'0' + time.hours%10;
    scr_tab[pos+2] = CHAR_COLOR<<8|':';
    scr_tab[pos+3] = CHAR_COLOR<<8|'0' + time.minutes/10;
    scr_tab[pos+4] = CHAR_COLOR<<8|'0' + time.minutes%10;
    scr_tab[pos+5] = CHAR_COLOR<<8|':';
    scr_tab[pos+6] = CHAR_COLOR<<8|'0' + time.seconds/10;
    scr_tab[pos+7] = CHAR_COLOR<<8|'0' + time.seconds%10;
}

void console_put_arbitrary(int pos, char c) {
    scr_tab[pos] = CHAR_COLOR<<8|c;
}

void console_put_arbitrary_color(int pos, char c, char color) {
    scr_tab[pos] = color<<8|c;
}   

