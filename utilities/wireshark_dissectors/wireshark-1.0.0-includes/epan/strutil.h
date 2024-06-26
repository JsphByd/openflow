/* strutil.h
 * String utility definitions
 *
 * $Id: strutil.h 3992 2008-06-10 03:13:11Z dgu $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef __STRUTIL_H__
#define __STRUTIL_H__

/* ... thus, config.h needs to be #included */

/** @file
 * String handling and conversion utilities.
 */

/** Given a pointer into a data buffer, and to the end of the buffer,
 *  find the end of the (putative) line at that position in the data
 *  buffer.
 *
 * @param data A pointer to the beginning of the data
 * @param dataend A pointer to the end of the data
 * @param eol A pointer that will receive the EOL location
 * @return A pointer to the EOL character(s) in "*eol".
 */
const guchar *find_line_end(const guchar *data, const guchar *dataend,
    const guchar **eol);

/** Get the length of the next token in a line, and the beginning of the
 *  next token after that (if any).
 * @param linep A pointer to the beginning of the line
 * @param lineend A pointer to the end of the line
 * @param next_token Receives the location of the next token
 * @return 0 if there is no next token.
 */
int        get_token_len(const guchar *linep, const guchar *lineend,
    const guchar **next_token);

/** Given a string, generate a string from it that shows non-printable
 *  characters as C-style escapes, and return a pointer to it.
 *
 * @param line A pointer to the input string
 * @param len The length of the input string
 * @return A pointer to the formatted string
 *
 * @see tvb_format_text()
 */
gchar*     format_text(const guchar *line, int len);

gchar*     format_text_wsp(const guchar *line, int len);

/** Turn an array of bytes into a string showing the bytes in hex.
 *
 * @param bd A pointer to the byte array
 * @param bd_len The length of the byte array
 * @return A pointer to the formatted string
 *
 * @see bytes_to_str_punct()
 */
gchar*     bytes_to_str(const guint8 *bd, int bd_len);

/** Turn an array of bytes into a string showing the bytes in hex,
 *  separated by a punctuation character.
 *
 * @param bd A pointer to the byte array
 * @param bd_len The length of the byte array
 * @param punct The punctuation character
 * @return A pointer to the formatted string
 *
 * @see bytes_to_str()
 */
gchar*     bytes_to_str_punct(const guint8 *bd, int bd_len, gchar punct);

/** Turn a string of hex digits with optional separators (defined by
 *  is_byte_sep() into a byte array.
 *
 * @param hex_str The string of hex digits.
 * @param bytes The GByteArray that will receive the bytes.  This
 *        must be initialized by the caller.
 * @param force_separators If set to TRUE, separators MUST exist between
 *        bytes.
 * @return True if the string was converted successfully
 */
gboolean   hex_str_to_bytes(const char *hex_str, GByteArray *bytes,
    gboolean force_separators);

/** Turn an RFC 3986 percent-encoded string into a byte array.
 *
 * @param uri_str The string of hex digits.
 * @param bytes The GByteArray that will receive the bytes.  This
 *        must be initialized by the caller.
 * @return True if the string was converted successfully
 * @see format_uri()
 */
gboolean   uri_str_to_bytes(const char *uri_str, GByteArray *bytes);

/** Turn a byte array into an RFC 3986 percent-encoded string.
 *
 * @param bytes The GByteArray that will receive the bytes.  This
 *        must be initialized by the caller.
 * @param reserved_chars Normally the "gen-delims" and "sub-delims"
 *        from RFC 3986 (":/?#[]@" and "!$&'()*+,;=" respectively)
 *        plus space (hex value 20) are treated as reserved characters.
 *        If this variable is non-NULL, its contents will be used
 *        instead.
 * @note Any non-printing character determined by isprint(), along
 *       with the % character itself are always reserved.
 * @see uri_str_to_bytes(),  format_text(), isprint()
 */
gchar*     format_uri(const GByteArray *bytes, const gchar *reserved_chars);

/** Turn a OID string representation (dot notation) into a byte array.
 *
 * @param oid_str The OID string (dot notaion).
 * @param bytes The GByteArray that will receive the bytes.  This
 *        must be initialized by the caller.
 * @return True if the string was converted successfully
 */
gboolean   oid_str_to_bytes(const char *oid_str, GByteArray *bytes);

/**
 * Create a copy of a GByteArray
 *
 * @param ba The byte array to be copied.
 * @return If ba exists, a freshly allocated copy.  NULL otherwise.
 *
 * XXX - Should this be in strutil.c?
 */
GByteArray *byte_array_dup(GByteArray *ba);

/**
 * Compare the contents of two GByteArrays
 *
 * @param ba1 A byte array
 * @param ba2 A byte array
 * @return If both arrays are non-NULL and their lengths are equal and
 *         their contents are equal, returns TRUE.  Otherwise, returns
 *         FALSE.
 *
 * XXX - Should this be in strutil.c?
 */
gboolean byte_array_equal(GByteArray *ba1, GByteArray *ba2);


/** Return a XML escaped representation of the unescaped string.
 *  The returned string must be freed when no longer in use.
 *
 * @param unescaped The unescaped string
 * @return An XML-escaped representation of the input string
 */
gchar*     xml_escape(const gchar *unescaped);

/**
 * Return the first occurrence of needle in haystack.
 * Algorithm copied from GNU's glibc 2.3.2 memcmp()
 *
 * @param haystack The data to search
 * @param haystack_len The length of the search data
 * @param needle The string to look for
 * @param needle_len The length of the search string
 * @return A pointer to the first occurrence of "needle" in
 *         "haystack".  If "needle" isn't found or is NULL, or if
 *         "needle_len" is 0, NULL is returned.
 */
const guint8 * epan_memmem(const guint8 *haystack, guint haystack_len,
		const guint8 *needle, guint needle_len);

/* Surround a string or a macro, resolved to a string, with double quotes */
#define _STRINGIFY(a)           # a
#define STRINGIFY(a)            _STRINGIFY(a)

/** Scan a string to make sure it's valid hex.
 *
 * @param string The string to validate
 * @param nbytes The length of the return buffer
 * @return A pointer to a buffer containing the converted raw bytes.  This
 *         buffer must be g_free()d by the caller.
 */
guint8 * convert_string_to_hex(const char *string, size_t *nbytes);

/** Prep a string for case-sensitive vs case-insensitive searching.
 *
 * @param string The search string
 * @param case_insensitive TRUE if case-insensitive, FALSE if not
 * @return A direct copy of the string if it's a case-sensitive search and
 * an uppercased version if not.  In either case the string must be g_free()d
 * by the caller.
 */
char * convert_string_case(const char *string, gboolean case_insensitive);

/** Finds the first occurence of string 'needle' in string 'haystack'.
 *  The matching is done in a case insensitive manner.
 *
 * @param haystack The string possibly containing the substring
 * @param needle The substring to be searched
 * @return A pointer into 'haystack' where 'needle' is first found.
 *   Otherwise it returns NULL.
 */
char * epan_strcasestr(const char *haystack, const char *needle);

/* g_strlcat() does not exist in GLib 1.2[.x] */
#if GLIB_MAJOR_VERSION < 2
gsize g_strlcat(gchar *dst, const gchar *src, gsize size);
gsize g_strlcpy1(gchar *dest, const gchar *src, gsize dest_size);
#endif

#if GLIB_MAJOR_VERSION < 2
/* g_ascii_isprint() does not exist in GLib 1.2[.x].
 * assume all codes >=0x20 and <0x80 are ASCII printables.
 */
#define g_ascii_isprint(c)						\
	(((c<0x20)||(c>=0x80))?FALSE:TRUE)
/* g_ascii_isxdigit() does not exist in Glib 1.2 */
#define g_ascii_isxdigit(c)						\
	( ((c>='0')&&(c<='9'))?TRUE:					\
		( ((c>='a')&&(c<='f'))?TRUE:				\
			(((c>='A')&&(c<='F'))?TRUE:FALSE) ) )

#endif

#if GLIB_MAJOR_VERSION < 2
/* g_byte_array_sized_new() doesnt exist in glib-1.2 */
GByteArray *g_byte_array_sized_new(guint reserved_size);
#endif

#endif /* __STRUTIL_H__ */
