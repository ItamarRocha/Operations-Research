using JuMP
using GLPK
using LinearAlgebra
import MathOptInterface # Replaces MathProgBase
const MOI = MathOptInterface
include("Data.jl")

# Int8: -128 -> 127
# Int16 : -32.768 -> 32.767
# Int32 : -2.147.483.648 -> 2.147.483.647
# Int64 : - 2^63 -> 2^63 - 1
# Float32, Float64, UINT8, UInt16, etccdddd

file = map(file->string(file), ARGS)

if(length(file) != 1)
  println("Wrong Pattern")
  println("julia main.jl instances/questao2.txt")
end

filepath = file[1]

capacity = Array{Int64,2}
d1 = Main.DataReader.Data(0,0,0,0,[],[])

capacity, d1 = Main.DataReader.readData(filepath)

costs = zeros(Int64,(d1.N_vertex, d1.N_vertex))

# costs ok | OK REAL
for i in 1:d1.N_vertex
  if i in d1.start_nodes
    costs[i,d1.end_node] = 1
  end
end

mcfp = Model(GLPK.Optimizer)

n = d1.N_vertex

@variable(mcfp, x[1:n,1:n], Int) #ok
#println(x)
# capacity  --------- taok
for i in 1:n
  for j in 1:n
    cap = capacity[i,j]
    #println("x[$i,$j] <= $cap") ---------- ta ok
    @constraint(mcfp, x[i,j] - capacity[i,j] <= 0)
  end
end

#sum 0
for i in 1:n
  if i in d1.start_nodes
    #println(d1.max_possible_flow[i]) taok
    #println(sum(x[i,j] - x[j,i] for j in 1:n)," ",d1.max_possible_flow[i]) taok
    @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == d1.max_possible_flow[i])
  elseif i == d1.end_node
    #println("end $i")
    max_end_flow = 0
    for node in d1.start_nodes
      max_end_flow += d1.max_possible_flow[node]
    end
    #println(sum(x[i,j] - x[j,i] for j in 1:n) , " == ", max_end_flow * -1 ) # essa parte ta ok vium
    @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == max_end_flow * -1)
  else
    #println(sum(x[i,j] - x[j,i] for j in 1:n))
    @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == 0) #ok tmb essa porra
  end
end

for i in 1:n
  @constraint(mcfp, x[i,i] == 0)
end

#@objective(mcfp, Min, sum(x[i,j] * costs[i,j] for i = 1:n, j = 1:n))
#println(sum(costs[i,j] * x[i,j] for i in 1:n, j in 1:n))
@objective(mcfp, Min, sum(costs[i,j] * x[i,j] for i in 1:n, j in 1:n))


JuMP.optimize!(mcfp)
@show JuMP.termination_status(mcfp) == MOI.OPTIMAL

@show objective_value(mcfp)
@show value.(x)