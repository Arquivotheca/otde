#ifndef GLOBAL_CONFIG
#define GLOBAL_CONFIG

#include <qcolor.h>

class global_config {
 public:
  QColor *colour_text;
  QColor *colour_info;
  QColor *colour_chan;
  QColor *colour_error;
};

extern global_config *kSircConfig;

#endif
