#include "libmx.h"

static void mx_write(char **lineptr, char *buf) {
    *lineptr = mx_realloc(*lineptr,
                          mx_s_strlen(*lineptr) + mx_s_strlen(buf));
    *lineptr = mx_strcat(*lineptr, buf);
}

static int mx_remembered_part(char **lineptr, char *readed,
                              r_line *l) {
    char *buf = mx_strdup(readed);
    int len = mx_strlen(buf);
    int r;
    int i = 0;

    for (int j = 0; buf[j] != '\0' ; j++, i++)
        if (buf[j] == l->delim) {
            buf[j++] = '\0';
            mx_write(lineptr, buf);
            for(r = 0; r < (len - i); readed[r++] = buf[j++]);
            l->flag = 1;
            mx_strdel(&buf);
            l->sum += i;
            return i;
        }
    mx_write(lineptr, buf);
    mx_strdel(&buf);
    mx_strdel(&readed);
    l->sum += i;
    return i;
}

static char *mx_read(char **lineptr, char *readed,
                     size_t buf_size, r_line *l) {
    for ( ; (l->chars = read(l->fd, l->buf, buf_size)) > 0; ) {
        l->buf[l->chars] = '\0';
        l->sum += l->chars;
        readed = mx_strdup(l->buf);
        for (size_t j = 0;
             j < buf_size || l->buf[j] != '\0'; j++, l->index++)
            if (l->buf[j] == l->delim) {
                l->buf[j++] = '\0';
                for (l->k = 0; l->k < buf_size - l->index % buf_size; )
                    readed[l->k++] = l->buf[j++];
                l->flag = 1;
                break;
            }
        mx_write(lineptr, l->buf);
        if (l->flag == 1)
            break;
        mx_strdel(&readed);
    }
    (int) l->chars == 0 && l->index == 0 ? l->index-- : l->index;
    return readed;
}

static r_line *struct_creator(size_t buf_size, char delim, const int fd) {
    r_line *l = (r_line *)malloc(sizeof(r_line));

    l->buf = mx_strnew(buf_size);
    l->flag = 0;
    l->index = 0;
    l->chars = 0;
    l->sum = 0;
    l->fd = fd;
    l->delim = delim;
    return l;
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    r_line *l = struct_creator(buf_size, delim, fd);
    static char *readed;
    int result;

    if (readed != NULL && mx_s_strlen(readed) == 0)
        mx_strdel(&readed);
    l->fd < 0 ? (l->index = -2, l->flag = 1) : l->index;
    if (readed != NULL && mx_strlen(readed) > 0)
        l->index = mx_remembered_part(lineptr, readed, l);
    if (l->flag == 0)
        readed = mx_read(lineptr, NULL, buf_size, l);
    result = l->index > l->sum ? l->sum : l->index;
    if (result == -1)
        mx_strdel(&readed);
    free(l->buf);
    free(l);
    return result;
}
