#ifndef OSM2PGSQL_OUTPUT_HPP
#define OSM2PGSQL_OUTPUT_HPP

/* Common output layer interface */

/* Each output layer must provide methods for
 * storing:
 * - Nodes (Points of interest etc)
 * - Way geometries
 * Associated tags: name, type etc.
*/

#include <osmium/thread/pool.hpp>

#include "options.hpp"

struct middle_query_t;
class db_copy_thread_t;

class output_t
{
public:
    static std::vector<std::shared_ptr<output_t>>
    create_outputs(std::shared_ptr<middle_query_t> const &mid,
                   options_t const &options);

    output_t(std::shared_ptr<middle_query_t> const &mid,
             options_t const &options);
    virtual ~output_t();

    virtual std::shared_ptr<output_t>
    clone(std::shared_ptr<middle_query_t> const &mid,
          std::shared_ptr<db_copy_thread_t> const &copy_thread) const = 0;

    virtual void start() = 0;
    virtual void stop(osmium::thread::Pool *pool) = 0;
    virtual void commit() = 0;

    virtual void stage2_proc() {}

    virtual bool need_forward_dependencies() const noexcept { return true; }

    virtual void pending_way(osmid_t id) = 0;
    virtual void pending_relation(osmid_t id) = 0;

    virtual void node_add(osmium::Node const &node) = 0;
    virtual void way_add(osmium::Way *way) = 0;
    virtual void relation_add(osmium::Relation const &rel) = 0;

    virtual void node_modify(osmium::Node const &node) = 0;
    virtual void way_modify(osmium::Way *way) = 0;
    virtual void relation_modify(osmium::Relation const &rel) = 0;

    virtual void node_delete(osmid_t id) = 0;
    virtual void way_delete(osmid_t id) = 0;
    virtual void relation_delete(osmid_t id) = 0;

    const options_t *get_options() const;

    virtual void merge_expire_trees(output_t *other);

protected:
    std::shared_ptr<middle_query_t> m_mid;
    const options_t m_options;
};

#endif // OSM2PGSQL_OUTPUT_HPP
