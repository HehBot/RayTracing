#ifndef READ_FROM_JSON_H
#define READ_FROM_JSON_H

#include <memory>

class camera;
class hittable_list;

std::shared_ptr<camera> read_from_json(char const* filename, hittable_list& world);

#endif // READ_FROM_JSON_H
