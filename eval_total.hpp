#include <boost/shared_ptr.hpp>
#include <sferes/dbg/dbg.hpp>
#include <sferes/stc.hpp>
#include <sferes/eval/eval.hpp>
#include <meta-cmaes/bottom_typedefs.hpp>

// eval class which allows us to:
// 1.
// 2.  count the total number of function evaluations

namespace sferes
{
namespace eval
{

SFERES_EVAL(EvalTotal, Eval)
{
public:
  EvalTotal() : _nb_evals(0) {}
  template <typename MetaIndiv>
  void eval(std::vector<boost::shared_ptr<MetaIndiv>> & pop, size_t begin, size_t end,
            const typename MetaIndiv::fit_t &fit_proto)
  {
    global::nb_evals = 0;
    //dbg::trace trace("eval", DBG_HERE);
    assert(pop.size());
    assert(begin < pop.size());
    assert(end <= pop.size());
    for (size_t i = begin; i < end; ++i)
    {
#ifdef PRINTING
      std::cout << "running the map for " + std::to_string(BottomParams::bottom_epochs) + " epochs" << std::endl;
      std::cout << typeid(pop[i]).name() << std::endl;
#endif
      //pop[i]->develop();// already developed into a map
      pop[i]->do_epochs(BottomParams::bottom_epochs);
#ifdef PRINTING
      std::cout << "evaluating meta-individual  " + std::to_string(i) << std::endl;
#endif

      pop[i]->fit().eval<MetaIndiv>(*pop[i]); // evaluate its recovered performance
    }
    _nb_evals += global::nb_evals;
#ifdef PRINTING
      std::cout<< "number of evaluations is now "<<_nb_evals<<std::endl;
#endif
  }
  unsigned nb_evals() const { return _nb_evals; }

protected:
  unsigned _nb_evals;
};
} // namespace eval
} // namespace sferes