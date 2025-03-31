#include "Functions.h"
#include "types/types.h"

AutomataCoroutine::pull_type makeAutomatasFromGraph(Graph& original) {
   return AutomataCoroutine::pull_type([original](AutomataCoroutine::push_type& yield) {
      for (int mask = 0; mask < 1 << original.m_vertices.size(); mask++) {
          Automata labeled;
          std::map<Graph::vertex_descriptor, Automata::vertex_descriptor> vertex_map;

          // vertexes copy
          for (auto [vi, vi_end] = boost::vertices(original); vi != vi_end; ++vi) {
              auto v_new = boost::add_vertex(labeled);
              vertex_map[*vi] = v_new;
          }

          unsigned int marked = 0;
          for (auto [ei, ei_end] = boost::edges(original); ei != ei_end; ++ei) {
              auto src = boost::source(*ei, original);
              auto tgt = boost::target(*ei, original);

              auto new_edge = boost::add_edge(vertex_map[src], vertex_map[tgt], labeled).first;
              labeled[new_edge].mark = (mask >> src ^ marked >> src) & 1 ? 'b' : 'a';
              marked |= 1 << src;
          }

          auto [vi, vi_end] = boost::vertices(labeled);
          for (auto it = vi; it != vi_end; ++it) {
              labeled[*it].node_id = *it;
          }

          yield(std::make_pair(mask, std::ref(labeled)));
      }
   });
}