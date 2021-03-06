#include <cassert>
#include <iostream>
#include <iterator>
#include <Array/Array.h>
#include <ReClaM/FFNet.h>
#include <ReClaM/createConnectionMatrix.h>

///NeuralNet is a derived class of FFNet
///to gain access to some protected methods of FFNet
struct NeuralNet : public FFNet
{
  NeuralNet(
    const int n_inputs,
    const int n_outputs,
    const Array<int> connection_matrix)
  : FFNet( n_inputs,n_outputs,connection_matrix) {}
  void Activate(const Array<double> &in)
  {
    this->activate(in);
  }
  unsigned int GetNumberOfNeurons()
  {
    return this->numberOfNeurons;
  }
};

NeuralNet CreateNet(
  const int n_inputs,
  const int n_hidden,
  const int n_outputs,
  const double init_weight_min,
  const double init_weight_max)
{
  //Create neural net connection matrix
  Array<int> connection_matrix;
  createConnectionMatrix(connection_matrix,n_inputs, n_hidden, n_outputs);
  //Create the feed-forward neural network
  NeuralNet n(n_inputs, n_outputs, connection_matrix);
  n.initWeights(init_weight_min,init_weight_max);
  return n;
}

void ShowXor(NeuralNet& n)
{
  std::cout << "Show how the neural net responds to input:\n";
  const unsigned int output_neuron_index = n.GetNumberOfNeurons() - 1;
  {
    std::vector<double> v(2);
    v[0] = 0.0;
    v[1] = 0.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    std::cout << "(" << v[0] << "," << v[1] << "): " << output
      << " (and should get to " << (v[0]!=v[1] ? "1.0" : "0.0") <<  ")'\n";
  }
  {
    std::vector<double> v(2);
    v[0] = 1.0;
    v[1] = 0.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    std::cout << "(" << v[0] << "," << v[1] << "): " << output
      << " (and should get to " << (v[0]!=v[1] ? "1.0" : "0.0") <<  ")'\n";
  }
  {
    std::vector<double> v(2);
    v[0] = 0.0;
    v[1] = 1.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    std::cout << "(" << v[0] << "," << v[1] << "): " << output
      << " (and should get to " << (v[0]!=v[1] ? "1.0" : "0.0") <<  ")'\n";
  }
  {
    std::vector<double> v(2);
    v[0] = 1.0;
    v[1] = 1.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    std::cout << "(" << v[0] << "," << v[1] << "): " << output
      << " (and should get to " << (v[0]!=v[1] ? "1.0" : "0.0") <<  ")'\n";
  }
}

double Rate_xor_success(NeuralNet& n)
{
  double rating = 4.0;
  const unsigned int output_neuron_index = n.GetNumberOfNeurons() - 1;
  {
    std::vector<double> v(2);
    v[0] = 0.0;
    v[1] = 0.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    rating -= std::fabs(0.0 - output);
  }
  {
    std::vector<double> v(2);
    v[0] = 1.0;
    v[1] = 0.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    rating -= std::fabs(1.0 - output);
  }
  {
    std::vector<double> v(2);
    v[0] = 0.0;
    v[1] = 1.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    rating -= std::fabs(1.0 - output);
  }
  {
    std::vector<double> v(2);
    v[0] = 1.0;
    v[1] = 1.0;
    Array<double> inputs(v);
    n.Activate(inputs);
    const double output = n.outputValue(output_neuron_index);
    rating -= std::fabs(0.0 - output);
  }
  return rating;
}

int main()
{
  NeuralNet net = CreateNet(2,2,1,-0.1,0.1);
  const double success = Rate_xor_success(net);
  ShowXor(net);
  std::cout << "Success [0.0 (worst),4.0(perfect)]: " << success << '\n';
}
