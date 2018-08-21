#include "Leaderboard.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "MapType.hpp"

namespace Portal2Boards {

bool Leaderboard::DoesExist()
{
    return bestTimeId != 0 && bestPortalsId != 0;
}

bool Leaderboard::TryFindByLevelName(const char* levelName, Leaderboard& leaderboard)
{
    for (auto& lb : Leaderboard::list) {
        if (!std::strcmp(levelName, lb.levelName)) {
            leaderboard = lb;
            return true;
        }
    }
    return false;
}

std::vector<Leaderboard> Leaderboard::list = std::vector<Leaderboard>{
    Leaderboard{ "sp_a1_intro1", "Container Ride", MapType::SinglePlayer, 62761, 0, 7 },
    Leaderboard{ "sp_a1_intro2", "Portal Carousel", MapType::SinglePlayer, 62758, 0, 7 },
    Leaderboard{ "sp_a1_intro3", "Portal Gun", MapType::SinglePlayer, 47458, 47459, 7 },
    Leaderboard{ "sp_a1_intro4", "Smooth Jazz", MapType::SinglePlayer, 47455, 47454, 7 },
    Leaderboard{ "sp_a1_intro5", "Cube Momentum", MapType::SinglePlayer, 47452, 47451, 7 },
    Leaderboard{ "sp_a1_intro6", "Future Starter", MapType::SinglePlayer, 47106, 47107, 7 },
    Leaderboard{ "sp_a1_intro7", "Secret Panel", MapType::SinglePlayer, 62763, 0, 7 },
    Leaderboard{ "sp_a1_wakeup", "Wakeup", MapType::SinglePlayer, 62759, 0, 7 },
    Leaderboard{ "sp_a2_intro", "Incinerator", MapType::SinglePlayer, 47735, 47734, 7 },
    Leaderboard{ "sp_a2_laser_intro", "Laser Intro", MapType::SinglePlayer, 62765, 0, 8 },
    Leaderboard{ "sp_a2_laser_stairs", "Laser Stairs", MapType::SinglePlayer, 47736, 47737, 8 },
    Leaderboard{ "sp_a2_dual_lasers", "Dual Lasers", MapType::SinglePlayer, 47738, 47739, 8 },
    Leaderboard{ "sp_a2_laser_over_goo", "Laser Over Goo", MapType::SinglePlayer, 47742, 47743, 8 },
    Leaderboard{ "sp_a2_catapult_intro", "Catapult Intro", MapType::SinglePlayer, 62767, 0, 8 },
    Leaderboard{ "sp_a2_trust_fling", "Trust Fling", MapType::SinglePlayer, 47744, 47745, 8 },
    Leaderboard{ "sp_a2_pit_flings", "Pit Flings", MapType::SinglePlayer, 47465, 47466, 8 },
    Leaderboard{ "sp_a2_fizzler_intro", "Fizzler Intro", MapType::SinglePlayer, 47746, 47747, 8 },
    Leaderboard{ "sp_a2_sphere_peek", "Ceiling Catapult", MapType::SinglePlayer, 47748, 47749, 9 },
    Leaderboard{ "sp_a2_ricochet", "Ricochet", MapType::SinglePlayer, 47751, 47750, 9 },
    Leaderboard{ "sp_a2_bridge_intro", "Bridge Intro", MapType::SinglePlayer, 47752, 47753, 9 },
    Leaderboard{ "sp_a2_bridge_the_gap", "Bridge the Gap", MapType::SinglePlayer, 47755, 47754, 9 },
    Leaderboard{ "sp_a2_turret_intro", "Turret Intro", MapType::SinglePlayer, 47756, 47757, 9 },
    Leaderboard{ "sp_a2_laser_relays", "Laser Relays", MapType::SinglePlayer, 47759, 47758, 9 },
    Leaderboard{ "sp_a2_turret_blocker", "Turret Blocker", MapType::SinglePlayer, 47760, 47761, 9 },
    Leaderboard{ "sp_a2_laser_vs_turret", "Laser vs Turret", MapType::SinglePlayer, 47763, 47762, 9 },
    Leaderboard{ "sp_a2_pull_the_rug", "Pull the Rug", MapType::SinglePlayer, 47764, 47765, 9 },
    Leaderboard{ "sp_a2_column_blocker", "Column Blocker", MapType::SinglePlayer, 47766, 47767, 10 },
    Leaderboard{ "sp_a2_laser_chaining", "Laser Chaining", MapType::SinglePlayer, 47768, 47769, 10 },
    Leaderboard{ "sp_a2_triple_laser", "Triple Laser", MapType::SinglePlayer, 47770, 47771, 10 },
    Leaderboard{ "sp_a2_bts1", "Jail Break", MapType::SinglePlayer, 47773, 47772, 10 },
    Leaderboard{ "sp_a2_bts2", "Escape", MapType::SinglePlayer, 47774, 47775, 10 },
    Leaderboard{ "sp_a2_bts3", "Turret Factory", MapType::SinglePlayer, 47776, 47777, 11 },
    Leaderboard{ "sp_a2_bts4", "Turret Sabotage", MapType::SinglePlayer, 47779, 47778, 11 },
    Leaderboard{ "sp_a2_bts5", "Neurotoxin Sabotage", MapType::SinglePlayer, 47780, 47781, 11 },
    Leaderboard{ "sp_a2_bts6", "Tube Ride", MapType::SinglePlayer, 0, 0, 11 },
    Leaderboard{ "sp_a2_core", "Core", MapType::SinglePlayer, 62771, 0, 11 },
    Leaderboard{ "sp_a3_00", "Long Fall", MapType::SinglePlayer, 0, 0, 12 },
    Leaderboard{ "sp_a3_01", "Underground", MapType::SinglePlayer, 47783, 47782, 12 },
    Leaderboard{ "sp_a3_03", "Cave Johnson", MapType::SinglePlayer, 47784, 47785, 12 },
    Leaderboard{ "sp_a3_jump_intro", "Repulsion Intro", MapType::SinglePlayer, 47787, 47786, 12 },
    Leaderboard{ "sp_a3_bomb_flings", "Bomb Flings", MapType::SinglePlayer, 47468, 47467, 12 },
    Leaderboard{ "sp_a3_crazy_box", "Crazy Box", MapType::SinglePlayer, 47469, 47470, 12 },
    Leaderboard{ "sp_a3_transition01", "PotatOS", MapType::SinglePlayer, 47472, 47471, 12 },
    Leaderboard{ "sp_a3_speed_ramp", "Propulsion Intro", MapType::SinglePlayer, 47791, 47792, 13 },
    Leaderboard{ "sp_a3_speed_flings", "Propulsion Flings", MapType::SinglePlayer, 47793, 47794, 13 },
    Leaderboard{ "sp_a3_portal_intro", "Conversion Intro", MapType::SinglePlayer, 47795, 47796, 13 },
    Leaderboard{ "sp_a3_end", "Three Gels", MapType::SinglePlayer, 47798, 47799, 13 },
    Leaderboard{ "sp_a4_intro", "Test", MapType::SinglePlayer, 88350, 0, 14 },
    Leaderboard{ "sp_a4_tb_intro", "Funnel Intro", MapType::SinglePlayer, 47800, 47801, 14 },
    Leaderboard{ "sp_a4_tb_trust_drop", "Ceiling Button", MapType::SinglePlayer, 47802, 47803, 14 },
    Leaderboard{ "sp_a4_tb_wall_button", "Wall Button", MapType::SinglePlayer, 47804, 47805, 14 },
    Leaderboard{ "sp_a4_tb_polarity", "Polarity", MapType::SinglePlayer, 47806, 47807, 14 },
    Leaderboard{ "sp_a4_tb_catch", "Funnel Catch", MapType::SinglePlayer, 47808, 47809, 14 },
    Leaderboard{ "sp_a4_stop_the_box", "Stop the Box", MapType::SinglePlayer, 47811, 47812, 14 },
    Leaderboard{ "sp_a4_laser_catapult", "Laser Catapult", MapType::SinglePlayer, 47813, 47814, 14 },
    Leaderboard{ "sp_a4_laser_platform", "Laser Platform", MapType::SinglePlayer, 47815, 47816, 14 },
    Leaderboard{ "sp_a4_speed_tb_catch", "Propulsion Catch", MapType::SinglePlayer, 47817, 47818, 14 },
    Leaderboard{ "sp_a4_jump_polarity", "Repulsion Polarity", MapType::SinglePlayer, 47819, 47820, 14 },
    Leaderboard{ "sp_a4_finale1", "Finale 1", MapType::SinglePlayer, 62776, 0, 15 },
    Leaderboard{ "sp_a4_finale2", "Finale 2", MapType::SinglePlayer, 47821, 47822, 15 },
    Leaderboard{ "sp_a4_finale3", "Finale 3", MapType::SinglePlayer, 47824, 47823, 15 },
    Leaderboard{ "sp_a4_finale4", "Finale 4", MapType::SinglePlayer, 47456, 47457, 15 },
    Leaderboard{ "sp_a5_credits", "Credits", MapType::SinglePlayer, 0, 0, -1 },
    Leaderboard{ "mp_coop_credits", "Credits", MapType::Cooperative, 0, 0, -1 },
    Leaderboard{ "mp_coop_start", "Start", MapType::Cooperative, 0, 0, 0 },
    Leaderboard{ "mp_coop_lobby_2", "Lobby", MapType::Cooperative, 0, 0, 0 },
    Leaderboard{ "mp_coop_doors", "Doors", MapType::Cooperative, 47741, 47740, 1 },
    Leaderboard{ "mp_coop_race_2", "Buttons", MapType::Cooperative, 47825, 47826, 1 },
    Leaderboard{ "mp_coop_laser_2", "Lasers", MapType::Cooperative, 47828, 47827, 1 },
    Leaderboard{ "mp_coop_rat_maze", "Rat Maze", MapType::Cooperative, 47829, 47830, 1 },
    Leaderboard{ "mp_coop_laser_crusher", "Laser Crusher", MapType::Cooperative, 45467, 45466, 1 },
    Leaderboard{ "mp_coop_teambts", "Behind The Scenes", MapType::Cooperative, 46362, 46361, 1 },
    Leaderboard{ "mp_coop_fling_3", "Flings", MapType::Cooperative, 47831, 47832, 2 },
    Leaderboard{ "mp_coop_infinifling_train", "Infinifling", MapType::Cooperative, 47833, 47834, 2 },
    Leaderboard{ "mp_coop_come_along", "Team Retrieval", MapType::Cooperative, 47835, 47836, 2 },
    Leaderboard{ "mp_coop_fling_1", "Vertical Flings", MapType::Cooperative, 47837, 47838, 2 },
    Leaderboard{ "mp_coop_catapult_1", "Catapults", MapType::Cooperative, 47840, 47839, 2 },
    Leaderboard{ "mp_coop_multifling_1", "Multifling", MapType::Cooperative, 47841, 47842, 2 },
    Leaderboard{ "mp_coop_fling_crushers", "Fling Crushers", MapType::Cooperative, 47844, 47843, 2 },
    Leaderboard{ "mp_coop_fan", "Industrial Fan", MapType::Cooperative, 47845, 47846, 2 },
    Leaderboard{ "mp_coop_wall_intro", "Cooperative Bridges", MapType::Cooperative, 47848, 47847, 3 },
    Leaderboard{ "mp_coop_wall_2", "Bridge Swap", MapType::Cooperative, 47849, 47850, 3 },
    Leaderboard{ "mp_coop_catapult_wall_intro", "Fling Block", MapType::Cooperative, 47854, 47855, 3 },
    Leaderboard{ "mp_coop_wall_block", "Catapult Block", MapType::Cooperative, 47856, 47857, 3 },
    Leaderboard{ "mp_coop_catapult_2", "Bridge Fling", MapType::Cooperative, 47858, 47859, 3 },
    Leaderboard{ "mp_coop_turret_walls", "Turret Walls", MapType::Cooperative, 47861, 47860, 3 },
    Leaderboard{ "mp_coop_turret_ball", "Turret Assassin", MapType::Cooperative, 52642, 52641, 3 },
    Leaderboard{ "mp_coop_wall_5", "Bridge Testing", MapType::Cooperative, 52660, 52659, 3 },
    Leaderboard{ "mp_coop_tbeam_redirect", "Cooperative Funnels", MapType::Cooperative, 52662, 52661, 4 },
    Leaderboard{ "mp_coop_tbeam_drill", "Funnel Drill", MapType::Cooperative, 52663, 52664, 4 },
    Leaderboard{ "mp_coop_tbeam_catch_grind_1", "Funnel Catch", MapType::Cooperative, 52665, 52666, 4 },
    Leaderboard{ "mp_coop_tbeam_laser_1", "Funnel Laser", MapType::Cooperative, 52667, 52668, 4 },
    Leaderboard{ "mp_coop_tbeam_polarity", "Cooperative Polarity", MapType::Cooperative, 52671, 52672, 4 },
    Leaderboard{ "mp_coop_tbeam_polarity2", "Funnel Hop", MapType::Cooperative, 52687, 52688, 4 },
    Leaderboard{ "mp_coop_tbeam_polarity3", "Advanced Polarity", MapType::Cooperative, 52689, 52690, 4 },
    Leaderboard{ "mp_coop_tbeam_maze", "Funnel Maze", MapType::Cooperative, 52691, 52692, 4 },
    Leaderboard{ "mp_coop_tbeam_end", "Turret Warehouse", MapType::Cooperative, 52777, 52778, 4 },
    Leaderboard{ "mp_coop_paint_come_along", "Repulsion Jumps", MapType::Cooperative, 52694, 52693, 5 },
    Leaderboard{ "mp_coop_paint_redirect", "Double Bounce", MapType::Cooperative, 52711, 52712, 5 },
    Leaderboard{ "mp_coop_paint_bridge", "Bridge Repulsion", MapType::Cooperative, 52714, 52713, 5 },
    Leaderboard{ "mp_coop_paint_walljumps", "Wall Repulsion", MapType::Cooperative, 52715, 52716, 5 },
    Leaderboard{ "mp_coop_paint_speed_fling", "Propulsion Crushers", MapType::Cooperative, 52717, 52718, 5 },
    Leaderboard{ "mp_coop_paint_red_racer", "Turret Ninja", MapType::Cooperative, 52735, 52736, 5 },
    Leaderboard{ "mp_coop_paint_speed_catch", "Propulsion Retrieval", MapType::Cooperative, 52738, 52737, 5 },
    Leaderboard{ "mp_coop_paint_longjump_intro", "Vault Entrance", MapType::Cooperative, 52740, 52739, 5 },
    Leaderboard{ "mp_coop_separation_1", "Separation", MapType::Cooperative, 49341, 49342, 6 },
    Leaderboard{ "mp_coop_tripleaxis", "Triple Axis", MapType::Cooperative, 49343, 49344, 6 },
    Leaderboard{ "mp_coop_catapult_catch", "Catapult Catch", MapType::Cooperative, 49345, 49346, 6 },
    Leaderboard{ "mp_coop_2paints_1bridge", "Bridge Gels", MapType::Cooperative, 49347, 49348, 6 },
    Leaderboard{ "mp_coop_paint_conversion", "Maintenance", MapType::Cooperative, 49349, 49350, 6 },
    Leaderboard{ "mp_coop_bridge_catch", "Bridge Catch", MapType::Cooperative, 49351, 49352, 6 },
    Leaderboard{ "mp_coop_laser_tbeam", "Double Lift", MapType::Cooperative, 52757, 52758, 6 },
    Leaderboard{ "mp_coop_paint_rat_maze", "Gel Maze", MapType::Cooperative, 52759, 52760, 6 },
    Leaderboard{ "mp_coop_paint_crazy_box", "Crazier Box", MapType::Cooperative, 48287, 48288, 6 },
    Leaderboard{ "e1912", "Super 8", MapType::Extras, 0, 0, -1 }
};
}
