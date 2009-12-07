
#ifndef NATIVESOLVER_H

#include <otkpp/lib/clone_ptr.hpp>
#include <otkpp/localsolvers/Solver.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <ostream>

using namespace boost::numeric::ublas;

/// Defines the base class of the implemented algorithms.
/**
 * This class is the base class for algorithms that find a (local) 
 * minimum of a function \f$f:\mathbb{R}^n\rightarrow\mathbb{R}\f$.
 */
class NativeSolver : public Solver
{
 public:
  /// Defines the status of the iteration.
  enum IterationStatus
  {
    ITERATION_CONTINUE,       ///< iteration is expected to progress towards a solution
    ITERATION_SUCCESS,        ///< iteration finished, success
    ITERATION_NO_PROGRESS,    ///< iteration is not making any progress
    ITERATION_OUT_OF_CONTROL  ///< iteration diverges or has reached infinity
  };
  
  struct State
  {
    double f;
    vector< double > x;
    matrix< double > X;
    
    virtual ~State() { }
    virtual State *clone() const = 0;
  };
  
  struct Results : public Solver::Results
  {
    std::vector< boost::shared_ptr< State > > states;
  };
  
  NativeSolver() { }
  virtual ~NativeSolver() { }
  
  /// Returns the current iterate \f$\mathbf{x}_{k}\f$.
  virtual const vector< double > getX() const;
  
  /// Returns an array of the current iterates.
  /**
   * If this solver generates multiple points per iteration, 
   * this method returns a matrix of them with each column 
   * containing one point. If this solver generates only 
   * one point per iteration step, this method returns 
   * a nx1 matrix identical to the vector returned by getX.
   */
  virtual const matrix< double > getXArray() const;
  
  /// Returns the current function value \f$f(\mathbf{x}_{k})\f$.
  virtual double getFVal() const;
  
  /// Returns the current gradient \f$\nabla f(\mathbf{x}_{k})\f$.
  virtual const vector< double > getGradient() const;
  
  /// Returns the current Hessian \f$H_{f}(\mathbf{x}_{k})\f$.
  virtual const matrix< double > getHessian() const;
  
  /// Returns the number of iterations since the last setup of this solver.
  unsigned int getNumIter() const;
  
  /// Returns the number of function evaluations since the last setup of this solver.
  unsigned int getNumFuncEval() const;
  
  /// Returns the number of gradient evaluations since the last setup of this solver.
  unsigned int getNumGradEval() const;
  
  /// Returns the number of Hessian evaluations since the last setup of this solver.
  unsigned int getNumHessEval() const;
  
  /// Returns the state of this solver.
  virtual const State &getState() const = 0;
  
  /// Returns the objective function associated with this solver.
  const Function &getObjectiveFunction() const;
  
  /// Does this solver have a built-in stopping criterion, or are custom stopping criteria allowed.
  virtual bool hasBuiltInStoppingCriterion() const = 0;
  
  /// Takes one iteration step and returns the status of the algorithm.
  IterationStatus iterate();
  
  boost::shared_ptr< Solver::Results > solve(Function &objFunc,
                                             const vector< double > &x0,
                                             const StoppingCriterion &stopCrit,
                                             const Setup &solverSetup = DefaultSetup(),
                                             const Constraints &C = NoConstraints(),
                                             bool timeTest = false);
 protected:
  unsigned int nIter_;
  
  State &getState_();
  virtual IterationStatus iterate_() = 0;
  virtual void setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const Setup &solverSetup = DefaultSetup(),
                      const Constraints &C = NoConstraints());
};

#define NATIVESOLVER_H

#endif
