/**
 * Tenemos que ir cargando los dispositivos con los que queremos interactuar usando numMayor y nimMinor (dos numeros que quedan libres). Para poder hacer eso tenemos que solicitar
 * al sistema operativo esos dos numeros con alloc_chrdev_region(), y hay q dar de alta con cdev_alloc(), cdev_init(), luego añadir cdev_add()
 * Para borrarlos cdev_del() y unregister_chrdev_region()
 * 
 * Usaremos las siguientes funciones: read, write, open, release
 * Hay que dar de alta estas funiones
 * 
 * -----CARGAR EL DISPOSITIVO-----
 * 1. MAYOR/MINOR_ SOLICITAR
 * int alloc_chrdev_region (dev_t *first, unsigned int firstminor, unsigned int count, char *name)
 * 2. REGISTRAR
 * int register_chrdev_region (dev_t first, unsigned int count, char *name);
 * 3. DAR DE ALTA
 * 
 * open y release se realiza siempre en las funciones entrada y salida
 * 
 * try_module_get(THIS_MODULE); incrementa un contador interno que lo que hace es que se le indica al SO que ese modulo esta siendo utilizado
 * module_put(THIS_MODULE); decrementa contador
 * 
 * /