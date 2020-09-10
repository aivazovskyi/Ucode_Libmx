#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub,
                        const char *replace) {
    char *new = NULL;

    if (str && sub && replace) {
        new = mx_strnew(mx_strlen(str) + (mx_strlen(replace) - mx_strlen(sub)) 
                        * mx_count_substr(str, sub));
        for (int i = 0; *str != '\0'; ) {
            if (!strncmp(str, sub, mx_strlen(sub))) {
                mx_strcat(new, replace);
                i += mx_strlen(replace);
                str += mx_strlen(sub);
            }
            else
                new[i++] = *str++;
        }
    }
    return new;
}

// char *mx_replace_substr(const char *str, const char *sub, 
//                         const char *replace){
//     char *result = NULL;
//     int res_len;
//     int i;
//     int j;

//     if (str && sub && replace) {
//         res_len = mx_strlen(str) + (mx_strlen(replace) - mx_strlen(sub)) * mx_count_substr(str, sub);
//         result = mx_strnew(res_len);
//         for ( ; str; ) {
//             j = mx_get_substr_index(str, sub);
//             for(i = 0; i < j; i++, str++, result++)
//                 *result = *str;
//             if (j >= 0) {
//                 for(i = 0; i < mx_strlen(replace); i++, result++)
//                     *result = replace[i];
//                 str += mx_strlen(sub);
//             }
//             if (j == -1) {
//                 while (*str != '\0')
//                     *result++ = *str++;
//                 break;
//             }
//         }
//         result -= res_len;
//     }
//     return result;
// }
