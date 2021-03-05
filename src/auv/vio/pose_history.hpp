#pragma once

#include <map>

#include "gtsam/geometry/Pose3.h"

namespace bm {
namespace vio {


template <typename Key>
class PoseHistory final {
 public:
  typedef std::map<Key, gtsam::Pose3> Map;

  PoseHistory() = default;

  Key NewestKey() const { return pose_map_.rbegin()->first; }
  Key OldestKey() const { return pose_map_.begin()->first; }
  size_t Size() const { return pose_map_.size(); }
  bool Empty() const { return pose_map_.empty(); }

  // Return the pose at key k.
  gtsam::Pose3 at(Key k) const { return pose_map_.at(k); }
  void Update(Key k, const gtsam::Pose3& pose) { pose_map_.emplace(k, std::move(pose)); }

  // Discard all poses *before* (but not equal to) the key k.
  void DiscardBefore(Key k)
  {
    // https://stackoverflow.com/questions/22874535/dependent-scope-need-typename-in-front
    typename Map::const_iterator it_first_item_gt_k = pose_map_.lower_bound(k);
    pose_map_.erase(pose_map_.begin(), it_first_item_gt_k);
  }

 private:
  Map pose_map_;
};


}
}
