library(ggplot2)
library(plotly)
library(networkD3)

# -------------------------------------------------------------------
# USER INTERFACE
# -------------------------------------------------------------------
ui <- fluidPage(
  titlePanel("Data Science Project 2017"),

  sidebarLayout(
    sidebarPanel(
      sliderInput("opacity", "Opacity", 0.8, min = 0.1,
                    max = 1, step = .1),
      
      selectInput("dataset", label = h4("Choose a dataset."),
                   choices = list("BH1987", "KB2009")),
      
      radioButtons("algorithim", label = h4("Choose an alogorithim."),
                   choices = list("Algorithim1" = 1, "Algorithim2" = 2,
                                  "Algorithim3" = 3),selected = 1)
    ),
    mainPanel(
      tabsetPanel(
        tabPanel("My Network", forceNetworkOutput("force1"))
      )
    )
  ),

  fluidRow(
    column(width = 4, class = "well",
      h4("Plotly Example"),
      plotlyOutput("plot1", height = 300)
      ),

    column(width = 4, class = "well",
      h4("Another Graph"),
      plotlyOutput("plot2", height = 300)
    ),

    column(width = 4, class = "well",
       h4("A Third Graph"),
       plotlyOutput("plot3", height = 300)
    )
   )
)

# -------------------------------------------------------------------
# SERVER PORTION
# -------------------------------------------------------------------
server <- function(input, output) {

  ##### NETWORK GRAPHS #####
  bhnodes <- read.csv("bh1987_nodes.csv")
  bhlinks <- read.csv("bh1987_links.csv")
  
  kbnodes <- read.csv("kb2009_nodes.csv")
  kblinks <- read.csv("kb2009_links.csv")
  
  all_nodes <- list(kbnodes, bhnodes)
  all_links <- list(kblinks, bhlinks)
  
  get_nodes <- reactive({switch(input$dataset,"KB2009" = kbnodes,"BH1987" = bhnodes)})
  get_links <- reactive({switch(input$dataset,"KB2009" = kblinks,"BH1987" = bhlinks)})
  
  output$force1 <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = "group", opacity = input$opacity,
                 fontSize = 16)
  })
  
  output$force2 <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Group = "origin", Nodesize = "degree", opacity = input$opacity,
                 fontSize = 16, zoom=input$checkbox)
  })

  ##### GRAPH STUFF #####
  ranges <- reactiveValues(x = NULL, y = NULL)
  ranges2 <- reactiveValues(x = NULL, y = NULL)

  data = read.csv("value_data.csv")
  df = data.frame(noise= data$noise, 
                  greedy=data$greedy_mean, 
                  pagerank=data$pagerank_value, 
                  optimal=data$optimal_value) 
  
  output$plot1 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~greedy, type="scatter", mode="lines")
  })
  
  output$plot2 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~optimal, type="scatter", mode="lines")
  })

  output$plot3 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~pagerank, type="scatter", mode="lines")
  })
}

shinyApp(ui, server)