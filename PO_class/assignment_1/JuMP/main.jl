using JuMP
using GLPK
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

function solver(filepath)
  capacity = Array{Int64,2}
  d1 = Main.DataReader.Data(0,0,0,0,[],[])

  capacity, d1 = Main.DataReader.readData(filepath)

  costs = zeros(Int64,(d1.N_vertex, d1.N_vertex))

  for i in 1:d1.N_vertex
    if i in d1.start_nodes
      costs[i,d1.end_node] = 1
    end
  end

  mcfp = Model(GLPK.Optimizer)

  n = d1.N_vertex

  @variable(mcfp, x[1:n,1:n] >= 0) #ok

  # capacity  --------- taok
  for i in 1:n
    for j in 1:n
      cap = capacity[i,j]
      @constraint(mcfp, x[i,j] - capacity[i,j] <= 0)
    end
  end

  #sum 0
  for i in 1:n
    if i in d1.start_nodes
      @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == d1.max_possible_flow[i])
    elseif i == d1.end_node
      global max_end_flow = 0
      for node in d1.start_nodes
        max_end_flow += d1.max_possible_flow[node]
      end
      @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == max_end_flow * -1)
    else
      @constraint(mcfp, sum(x[i,j] - x[j,i] for j in 1:n) == 0) #ok tmb essa porra
    end
  end


  @objective(mcfp, Min, sum(costs[i,j] * x[i,j] for i in 1:n, j in 1:n))

  #println(mcfp)
  function solve_mcfp(mcfp)
    JuMP.optimize!(mcfp)
    @show JuMP.termination_status(mcfp)
    println("Max Flow = ", max_end_flow - objective_value(mcfp))

  end
  @time solve_mcfp(mcfp)
end

solver("../material/instance1.txt")
solver(filepath)
solver("../material/instance1.txt")
