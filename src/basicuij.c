#include "basicuij.h"
#include <curl/curl.h>
#include <net_connection.h>

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *label;
	Evas_Object *dynamic_label;
	Evas_Object *boxybeat;
	Evas_Object *secondlabel;
	Evas_Object *naviframe;
} appdata_s;

//typedef CURLsslset cset;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}


/*
 * callback for button press.  GO!
 * */
void clicked_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	/* Box */
	Evas_Object *bg_box = elm_box_add(ad->naviframe);
	ad->dynamic_label = elm_label_add(bg_box);
	evas_object_size_hint_weight_set(ad->dynamic_label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->dynamic_label, EVAS_HINT_FILL, 0.5);
	elm_object_text_set(ad->dynamic_label, "<align=center> Box 2 <br/> curl results from callback <br/> before timeout.<br/>--no data--</align>");
	elm_label_line_wrap_set(ad->dynamic_label, ELM_WRAP_WORD);
	elm_label_slide_go(ad->dynamic_label);
	elm_box_pack_end(bg_box, ad->dynamic_label);
	evas_object_show(ad->dynamic_label);
	evas_object_show(bg_box);

	/* Back Button */
	Evas_Object *bg_back_button = elm_button_add(bg_box);
	evas_object_size_hint_weight_set(bg_back_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(bg_back_button, EVAS_HINT_FILL, 1);
	elm_object_text_set(bg_back_button, "Go Back2!");
	evas_object_smart_callback_add(bg_back_button, "clicked", clicked_back_cb, ad);
	elm_box_pack_end(bg_box, bg_back_button);
	evas_object_show(bg_back_button);
	elm_naviframe_item_push(ad->naviframe, "bg box in naviframe", NULL, NULL, bg_box, NULL);

	/* Box widget */
	Evas_Object *box = elm_box_add(ad->naviframe);
	elm_box_horizontal_set(box, EINA_FALSE);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_naviframe_item_push(ad->naviframe, "button added naviframe", NULL, NULL, box, NULL);
	evas_object_show(box);

	/* Back Button */
	Evas_Object *back_button = elm_button_add(box);
	evas_object_size_hint_weight_set(back_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(back_button, EVAS_HINT_FILL, 1);
	elm_object_text_set(back_button, "Go Back!");
	evas_object_smart_callback_add(back_button, "clicked", clicked_back_cb, ad);
	elm_box_pack_end(box, back_button);
	evas_object_show(back_button);

	/* Show Button */
	Evas_Object *show_button = elm_button_add(box);
	evas_object_size_hint_weight_set(show_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(show_button, EVAS_HINT_FILL, 1);
	elm_object_text_set(show_button, "SHOW!");
	evas_object_smart_callback_add(show_button, "clicked", clicked_threads_show, ad);
	elm_box_pack_end(box, show_button);
	evas_object_show(show_button);
}

void clicked_back_cb(void *data, Evas_Object *ojb, void *event_info){
	//pop frame, exposing previous page
	appdata_s *ad = data;
	elm_naviframe_item_pop(ad->naviframe);
}

void clicked_threads_show(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	char ui_message_str[65] = "<align=center> Box 2<br/>max threads = ";
	char jmax_threads[sizeof(int)] = "";
	snprintf(jmax_threads, sizeof jmax_threads, "%d", ecore_thread_max_get());
	dlog_print(DLOG_INFO, "USR_TAG", "max threads =  %s", jmax_threads);
	strcat(ui_message_str, jmax_threads);
	strcat(ui_message_str, "</align>");
	elm_object_text_set(ad->dynamic_label, ui_message_str);

	CURL *curl;
	CURLcode res;
	//cset global_curl_settings = curl_global_sslset(CURLSSLBACKEND_OPENSSL, NULL, NULL);
	curl = curl_easy_init();
	if (curl == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "curl no init.");
	    }
	connection_h connection;
	int conn_err;
	conn_err = connection_create(&connection);
	if (conn_err != CONNECTION_ERROR_NONE) {
	    /* Error handling */
		dlog_print(DLOG_ERROR, LOG_TAG, "connection error on create ");
	    return;
	}

	else{
		struct curl_slist *headers = NULL;
		curl_slist_append(headers, "Accept: application/json");
		curl_slist_append(headers, "Content-Type: application/json");
		curl_slist_append(headers, "charset: utf-8");
		curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.105:8000/data_tags_list/");
		//curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.105:8000/data_tags_list/");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");
		res = curl_easy_perform(curl);
		if (res != CURLE_OK){
			dlog_print(DLOG_ERROR, LOG_TAG, "curl perform:  %d", res);
			dlog_print(DLOG_ERROR, LOG_TAG, "NOT OK reply:  %d", res);
		}
		else{

			dlog_print(DLOG_ERROR, LOG_TAG, "OK reply:  %d", res);
		}
	}
	curl_easy_cleanup(curl);
	connection_unset_proxy_address_changed_cb(connection);
	connection_destroy(connection);

}

static void create_base_gui(appdata_s *ad)
{
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);


	/* Naviframe add */
	ad->naviframe = elm_naviframe_add(ad->conform);
	elm_object_content_set(ad->conform, ad->naviframe);
	evas_object_show(ad->conform);


	/* Box widget */
	ad->boxybeat = elm_box_add(ad->naviframe);
	elm_box_horizontal_set(ad->boxybeat, EINA_FALSE);
	evas_object_size_hint_weight_set(ad->boxybeat, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->boxybeat, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_naviframe_item_push(ad->naviframe, "Box Title on naviframe1", NULL, NULL, ad->boxybeat, NULL);
	evas_object_show(ad->boxybeat);

	/* Label */

	Evas_Object *label = elm_label_add(ad->boxybeat);
	evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(label, EVAS_HINT_FILL, 0.5);
	elm_object_text_set(label, "<align=center>AOFHTI1GN6RFB5E</align>");
	elm_box_pack_end(ad->boxybeat, label);
	evas_object_show(label);

	/* second Label*/

	Evas_Object *secondlabel = elm_label_add(ad->boxybeat);
	evas_object_size_hint_weight_set(secondlabel, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(secondlabel, EVAS_HINT_FILL, 0.8);
	elm_object_text_set(secondlabel, "<align=center>BOX 1</align>");
	elm_box_pack_end(ad->boxybeat, secondlabel);
	evas_object_show(secondlabel);

	/* Button */
	Evas_Object *button = elm_button_add(ad->boxybeat);
	evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(button, EVAS_HINT_FILL, 1);
	elm_object_text_set(button, "Go!");
	evas_object_smart_callback_add(button, "clicked", clicked_cb, ad);
	elm_box_pack_end(ad->boxybeat, button);
	evas_object_show(button);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
	int jmax_threads = ecore_thread_max_get();
	dlog_print(DLOG_INFO, "USR_TAG", "LOW MEM : max threads =  %d", jmax_threads);
}

int main(int argc, char *argv[])
{
	appdata_s ad = {0,};//evas objects
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
