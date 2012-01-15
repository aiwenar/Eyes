#ifndef HARDWARE_HXX
#define HARDWARE_HXX


class hardware
{
public:
    bool                    special_batname,
                            special_thername;
    double                  C_LOAD              (),
                            M_LOAD              ();
    int                     U_TIME              (),
                            bateria             (),
                            bat_plugged         ();
    unsigned int            battery_capacity;
    unsigned short          P_LIST              (),
                            temperatura         (),
                            emubat              (string path),
                            emustat             (string path),
                            (hardware::*src_batt_now)     (string),
                            (hardware::*src_batt_state)   (string),
                            (hardware::*src_temp)         (string),
                            proc_temp           (string path),
                            proc_bat_now        (string path),
                            proc_bat_full       (string path),
                            proc_bat_state      (string path),
                            sys_temp            (string path),
                            sys_bat_uni         (string path),
                            sys_bat_state       (string path);
    char                    final_temp_solution,
                            final_now_solution,
                            final_full_solution,
                            final_state_solution;
    void                    system_check        ();
    string                  final_path_temp,
                            final_path_now,
                            final_path_full,
                            final_path_state,
                            cfg_battname,
                            cfg_thername,
                            get_file            (char* path);
};

extern          hardware        HRDWR;
extern          sdate           get_time    ();
extern          unital          temperature;
extern          percental       battery;
extern          unsigned int    battery_capacity;
extern          data_source     d_src;

using namespace std;

#endif // HARDWARE_HXX
