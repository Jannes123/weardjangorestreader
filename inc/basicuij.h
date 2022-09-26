#ifndef __basicuij_H__
#define __basicuij_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <elm_object.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "basicuij"

#if !defined(PACKAGE)
#define PACKAGE "org.example.basicuij"
#endif

void clicked_back_cb(void *data, Evas_Object *ojb, void *event_info);
void clicked_threads_show(void *data, Evas_Object *obj, void *event_info);

#endif /* __basicuij_H__ */
