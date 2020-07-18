module DataReader
export Data
  mutable struct Data
    N_vertex::Int64
    N_edges::Int64
    initial_node::Int64
    end_node::Int64
    max_possible_flow::Array{Int64}
    #capacity::Array{Int64}(undef, 0, 3)
    start_nodes::Array{Int64}
  end

  N_vertex(d::Data) = d.N_vertex
  N_edges(d::Data) = d.N_edges
  initial_node(d::Data) = d.initial_node
  end_node(d::Data) = d.end_node
  max_possible_flow(d::Data) = d.max_possible_flow
  #capacity(d::Data) = d.capacity
  start_nodes(d::Data) = d.start_nodes

  d1 = Data(0,0,0,0,[],[]) # initializing all with 0

  #println(d1.N_vertex)

  function readData(filepath)
    open(filepath) do f
      #while !eof(f)
      d1.N_vertex = parse(Int64,readline(f))
      d1.N_edges = parse(Int64,readline(f))
      d1.initial_node = parse(Int64,readline(f))
      d1.end_node = parse(Int64,readline(f))
    
      d1.start_nodes = zeros(Int64, d1.N_vertex)
      
      global capacity = zeros(Int64,(d1.N_vertex, d1.N_vertex))
      # for n in 1:d1.N_vertex
      #   d1.max_possible_flow[n] = 1
      #   println(d1.max_possible_flow[n])
      # end

      vertices1 = Any[]
      vertices2 = Any[]

      for n in 1:d1.N_edges
        line = readline(f)
        vertex1 = parse(Int64,split(line, " ")[1])
        vertex2 = parse(Int64,split(line, " ")[2])
        cap = parse(Int64,split(line, " ")[3])
        #println("Vertex --> x[$vertex1,$vertex2] = $cap")
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

      # for startN in vertices1
      #   println(startN)
      # end

      d1.start_nodes = vertices1
      #println(length(d1.start_nodes))

      # putting the max_possible_flow
      d1.max_possible_flow = zeros(Int64,length(d1.start_nodes))

      for i in d1.start_nodes
        for j in 1:d1.N_vertex
          d1.max_possible_flow[i] += capacity[i,j]
        end
        capacity[i,d1.end_node] = d1.max_possible_flow[i] #pfcm
      end
    end
    # for i in 1:d1.N_vertex
    #   for j in 1:d1.N_vertex
    #     print(capacity[i,j], " ")
    #   end
    #   println("")
    # end
    return capacity, d1
  end
end