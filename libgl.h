#ifndef LIBGL_H_
#define LIBGL_H_

void __attribute__ ((constructor)) load(void);
void __attribute__ ((destructor)) unload(void);

void load_all();
void* load_lib(const char *path);

void poc();

#endif /* LIBGL_H_ */
