#ifndef PNC_MAP_GLOBAL_H_
#define PNC_MAP_GLOBAL_H_

#include "pnc_map_creator_base.h"

namespace Planning
{
    class PNCMapGlobal : public PNCMapCreatorBase //全局地图
    {
    public:
        PNCMapGlobal();
        PNCMap create_pnc_map(); //创建pnc地图

    private:
        void init_pnc_map(); //初始化pnc地图
        void draw_straight_x(const double &lengtg, const double &plus_flag, const double &ration = 1.0); //沿x轴方向画图

    };
}//namespace Planning
#endif //PNC_MAP_GLOBAL_H_
