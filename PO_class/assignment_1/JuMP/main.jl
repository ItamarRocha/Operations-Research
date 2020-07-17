using JuMP
using GLPK
using LinearAlgebra
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

# costs ok
for i in 1:d1.N_vertex
  if i in d1.start_nodes
    costs[i,d1.end_node] = 1
  end
end

mcfp = Model(GLPK.Optimizer)

n = d1.N_vertex

@variable(mcfp, x[1:n,1:n], Bin)

# capacity  --------- taok
for i in 1:d1.N_vertex
  for j in 1:d1.N_vertex
    cap = capacity[i,j]
    #println("x[$i,$j] = $cap")
    @constraint(mcfp, x[i,j] - capacity[i,j] <= 0)
  end
end

#sum 0
for i in 1:d1.N_vertex
  if i in d1.start_nodes
    #println(d1.max_possible_flow[i]) taok
    @constraint(mcfp, sum(x[i,:] - x[:,i]) == d1.max_possible_flow[i])
  elseif i == d1.end_node
    #println("end $i")
    max_end_flow = 0
    for node in d1.start_nodes
      max_end_flow += d1.max_possible_flow[node]
    end
    #println(max_end_flow) essa parte ta ok vium
    @constraint(mcfp, sum(x[i,:] - x[:,i]) == max_end_flow * -1)
  else
    #println("transshipment $i")
    @constraint(mcfp, sum(x[i,:] - x[:,i]) == 0)
  end
end

#@objective(mcfp, Min, sum(x[i,j] * costs[i,j] for i = 1:d1.N_vertex, j = 1:d1.N_vertex))
@objective(mcfp, Min, x[1,10] * costs[1,10] + x[2,10] * costs[2,10] + x[3,10] * costs[3,10])
optimize!(mcfp)
@show objective_value(mcfp)
#@show value.(x)