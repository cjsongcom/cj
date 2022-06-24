
#ifndef _CJSCH_TRIGGER_H_
#define _CJSCH_TRIGGER_H_


typedef enum cjsch_trigger_start_type_t {
    CJSCH_TRIGGER_START_TYPE_NONE,
    CJSCH_TRIGGER_START_TYPE_SCHEDULE,
    CJSCH_TRIGGER_START_TYPE_PROC_START,
    CJSCH_TRIGGER_START_TYPE_PROC_TICK,
    CJSCH_TRIGGER_START_TYPE_PROC_EVENT,
} cjsch_trigger_start_type;


typedef struct cjsch_trigger_period_t {

    // period
    cj_period_type						type;

    union {
        cj_per_every_day				every_day;
        cj_per_every_week				every_week;
        cj_per_every_month				every_month;
    } dat;

} cjsch_trigger_period;


typedef struct cjsch_trigger_advance_t {

    // delayed start
    cj_duration						delay_start_duration;

    // loop option
    cjbool							is_loop_enable;
    cj_interval						loop_interval;
    cj_duration						loop_duration;
    cjbool							terminate_all_at_loop_exit;

    // max execution
    cjbool							is_max_execution_enable;
    cj_duration						max_execution_duration;
    cjbool							terminate_at_max_execution_exit;

} cjsch_trigger_advance;


typedef struct cjsch_trigger_t {

    cjsch_ch                        name[CJ_MAX_CJSCH_TRIGGER_NAME];
    cjsch_ch                        class[CJ_MAX_CJSCH_CLASS_NAME];

    cji                             rev;
    cjbool                          enable;

    cjsch_trigger_start_type        st_type;
    cjdatetime                      st_datetime;

    cjbool                          expire_enable;
    cjdatetime                      expire_datetime;

    /////////////////////////////////////////////////
    cjsch_trigger_period            period;

    /////////////////////////////////////////////////
    cjbool                          advance_enable;
    cjsch_trigger_advance           advance;

} cjsch_trigger;


CJEXTERNC void cjsch_trigger_destroy(cjsch_trigger* trig);




#endif


/*
  triggers:
    - name: tigger00
      class: cjsch_trigger
      rev: 1
      enable: true
      start:
        type: schedule
        opt:
          schedule:
          on_start:
          tick:
          event:
        # iso8601 datetime
        # utc time: yyyy-mm-ddThh:mm:ss.msZ
        # local time: yyyy-mm-ddThh:mm:ss.ms
        # local time,with zone offset(+09:00):
        # > yyyy-mm-ddThh:mm:ss.ms+09:00
        datetime: "2022-06-05T15:53:16.000+09:00"
        expire:
          enable: 1
          datetime: "2022-06-05T15:53:16.000+09:00"

      period:
        rev: 1
        # once, every_day, every_week, every_month
        type: every_month
        opt:
          once:
          every_day:
            per_day: 1
          every_week:
            per_week: 1
            # [ sun, mon, tue, wed, thu, fri, sat ]
            wdays: [ sun, fri ]
          every_month:
            # [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0 ] , 0:last month
            months: [ 2, 10, 12 ]
            # day, week
            per_type: week
            per_opt:
              # [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
              #   13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
              #   23, 24, 25, 26, 27, 28, 29, 30, 31, 0 ]
              # 0: last day
              day:
                days: [ 1, 30 ]
              week:
                # [ 1, 2, 3, 4, 0] , 1:first week, 2:second week, .. 0: last day
                weeks: [ 2, 3]
                # [ sun, mon, tue, wed, thu, fri, sat ]
                wdays: [ mon ]

      advance:
        rev: 1
        enable: 1
        delay_start:
          # 0 30s 1m 30m 1h 12h 1d
          duration: 20
        loop:
          enable: 1
          # 5m, 10m, 15m, 30m, 1h
          interval: 12h
          # inf 15m 30m 1h 12h 1d
          duration: 2d
          terminate_all: 1
        max_execution:
          enable: 0
          # 30m 1h 2h 4h 8h 12h 1d 3d
          duration: 3d
          terminate: 1
*/
