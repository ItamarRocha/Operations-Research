module DataReader
export Data
  mutable struct Data
    N_vertex::Int64
    N_edges::Int64
    initial_node::Int64
    end_node::Int64
    max_possible_flow::Array{Int64}
    start_nodes::Array{Int64}
  end

  N_vertex(d::Data) = d.N_vertex
  N_edges(d::Data) = d.N_edges
  initial_node(d::Data) = d.initial_node
  end_node(d::Data) = d.end_node
  max_possible_flow(d::Data) = d.max_possible_flow
  start_nodes(d::Data) = d.start_nodes

  d1 = Data(0,0,0,0,[],[]) # initializing all with 0

  function readData(filepath)
    open(filepath) do f
      d1.N_vertex = parse(Int64,readline(f))
      d1.N_edges = parse(Int64,readline(f))
      d1.initial_node = parse(Int64,readline(f))
      d1.end_node = parse(Int64,readline(f))
    
      d1.start_nodes = zeros(Int64, d1.N_vertex)
      
      global capacity = zeros(Int64,(d1.N_vertex, d1.N_vertex))

      vertices1 = Any[]
      vertices2 = Any[]

      for n in 1:d1.N_edges
        line = readline(f)
        vertex1 = parse(Int64,split(line, " ")[1])
        vertex2 = parse(Int64,split(line, " ")[2])
        cap = parse(Int64,split(line, " ")[3])

        capacity[vertex1, vertex2] = cap

        if ! (vertex1 in vertices1)
          push!(vertices1, vertex1)
        end

        if ! (vertex2 in vertices2)
          push!(vertices2, vertex2)
        end
      
      end

      for element in vertices2
        if element in vertices1
          filter!(e->e!=element,vertices1)
        end
      end

      d1.start_nodes = vertices1

      d1.max_possible_flow = zeros(Int64,length(d1.start_nodes))

      for i in d1.start_nodes
        for j in 1:d1.N_vertex
          d1.max_possible_flow[i] += capacity[i,j]
        end
        capacity[i,d1.end_node] = d1.max_possible_flow[i] #pfcm
      end
    end

    return capacity, d1
  end
end