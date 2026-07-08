#include "main.h"

#include "FileType.h"

static const std::unordered_map<std::string_view, std::string_view> mimeTypes = {
    // HTML
    {".html", "text/html"},
    {".htm", "text/html"},

    // Stylesheets
    {".css", "text/css"},

    // JavaScript
    {".js", "application/javascript"},
    {".mjs", "application/javascript"},

    // JSON / XML
    {".json", "application/json"},
    {".xml", "application/xml"},

    // Text
    {".txt", "text/plain"},
    {".csv", "text/csv"},
    {".md", "text/markdown"},
    {".log", "text/plain"},

    // Images
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".bmp", "image/bmp"},
    {".ico", "image/x-icon"},
    {".svg", "image/svg+xml"},
    {".webp", "image/webp"},
    {".tif", "image/tiff"},
    {".tiff", "image/tiff"},
    {".avif", "image/avif"},

    // Audio
    {".mp3", "audio/mpeg"},
    {".wav", "audio/wav"},
    {".ogg", "audio/ogg"},
    {".oga", "audio/ogg"},
    {".aac", "audio/aac"},
    {".flac", "audio/flac"},
    {".m4a", "audio/mp4"},
    {".weba", "audio/webm"},

    // Video
    {".mp4", "video/mp4"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".webm", "video/webm"},
    {".avi", "video/x-msvideo"},
    {".mov", "video/quicktime"},
    {".wmv", "video/x-ms-wmv"},
    {".mkv", "video/x-matroska"},
    {".3gp", "video/3gpp"},

    // Fonts
    {".ttf", "font/ttf"},
    {".otf", "font/otf"},
    {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".eot", "application/vnd.ms-fontobject"},

    // Documents
    {".pdf", "application/pdf"},
    {".doc", "application/msword"},
    {".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {".xls", "application/vnd.ms-excel"},
    {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},

    // Archives
    {".zip", "application/zip"},
    {".rar", "application/vnd.rar"},
    {".7z", "application/x-7z-compressed"},
    {".tar", "application/x-tar"},
    {".gz", "application/gzip"},
    {".bz2", "application/x-bzip2"},
    {".xz", "application/x-xz"},

    // Executables / binaries
    {".exe", "application/octet-stream"},
    {".dll", "application/octet-stream"},
    {".bin", "application/octet-stream"},
    {".so", "application/octet-stream"},

    // Web manifests
    {".webmanifest", "application/manifest+json"},

    // WebAssembly
    {".wasm", "application/wasm"},

    // RSS / Atom
    {".rss", "application/rss+xml"},
    {".atom", "application/atom+xml"},

    // Calendar
    {".ics", "text/calendar"},

    // Misc
    {".rtf", "application/rtf"},
    {".yaml", "application/yaml"},
    {".yml", "application/yaml"},
    {".apk", "application/vnd.android.package-archive"},
    {".dmg", "application/x-apple-diskimage"},
    {".iso", "application/x-iso9660-image"}
};

const char* GetMimeType(std::string_view extension)
{
    std::string ext(extension);
    std::transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c) { return std::tolower(c); });

    auto it = mimeTypes.find(ext);

    if (it != mimeTypes.end())
        return it->second.data();

    return "application/octet-stream";
}