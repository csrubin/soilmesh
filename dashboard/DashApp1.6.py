import dash
import csv
import datetime

import dash_core_components as dcc
import dash_html_components as html
import dash_table
#import dash_table_experiments as dt
import pandas as pd
import plotly.graph_objs as go
from plotly.graph_objs import *
from dash.dependencies import Input, Output

#external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']
#app = dash.Dash(__name__)
#app.css.append_css({'external_url': 'https://codepen.io/amyoshino/pen/jzXypZ.css'})

external_stylesheets = ['https://codepen.io/amyoshino/pen/jzXypZ.css']
app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

#app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

#df = pd.read_csv('WORK2.csv')

df1 = pd.read_csv('NODE1.csv')
df2 = pd.read_csv('NODE2.csv')
df3 = pd.read_csv('NODE3.csv')
#with open('WORK2.csv') as f:
#    last3 = list(f)[-3:]

#print(last3)

#with open('created.csv', 'wb') as csvfile:
#    filewriter = csv.writer(csvfile, delimiter = ',')
#    filewriter.writerow(last3)



data = [] #Buffer list
with open("NODE1.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reader:
        data.append(row)
        break
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)
            
with open("NODE2.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)
            
with open("NODE3.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i=0
    for row in reversed(list(csv.reader(input_file))):
        i+=1
        if i < 2:
            data.append(row)

with open("the_new_csv.csv", "w+") as to_file:
    writer = csv.writer(to_file, delimiter=',')
    row = data[0]
    #print(row)
    writer.writerow(row)

    for new_row in data:
        if new_row[0] == '1':
           writer.writerow(new_row)


    for new_row in data:
        if new_row[0] == '2':
            writer.writerow(new_row)

    for new_row in data:
        if new_row[0] == '3':
            writer.writerow(new_row)

data2 = [] #Buffer list
with open("NODE1.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    for row in list(csv.reader(input_file)):
        data2.append(row)
            
with open("NODE2.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i = 0
    for row in list(csv.reader(input_file)):
        if i != 0:
            data2.append(row)
        i+=1
            
with open("NODE3.csv", "r") as input_file:
    reader = csv.reader(input_file, delimiter=',')
    i = 0
    for row in list(csv.reader(input_file)):
        if i != 0:
            data2.append(row)
        i+=1
        
        
with open("the_new_csv2.csv", "w+") as to_file:
    writer = csv.writer(to_file, delimiter=',')
    for new_row in data2:
           writer.writerow(new_row)


dfTable = pd.read_csv('the_new_csv.csv')
df = pd.read_csv('the_new_csv2.csv')
dfMap = pd.read_csv('https://raw.githubusercontent.com/plotly/datasets/master/2011_february_us_airport_traffic.csv')
dfMap['text'] = dfMap['airport'] + '' + dfMap['city'] + ', ' + dfMap['state'] + '' + 'Arrivals: ' + dfMap['cnt'].astype(str)

layout_table = dict(
    autosize=True,
    height=500,
    font=dict(color="#191A1A"),
    titlefont=dict(color="#191A1A", size='14'),
    margin=dict(
        l=35,
        r=35,
        b=35,
        t=45
    ),
    hovermode="closest",
    plot_bgcolor='#fffcfc',
    paper_bgcolor='#fffcfc',
    legend=dict(font=dict(size=10), orientation='h'),
)
layout_table['font-size'] = '18'
layout_table['margin-top'] = '120'
layout_table['margin-left'] = '60'



app.layout = html.Div([

    html.Div(
        [

        html.Div(
            [
                    html.P('Choose Time Frame:'),
                    dcc.RadioItems(
                            id = 'TimeFrame',
                            options=[
                                {'label': '10 Seconds', 'value': 'DY'},
                                {'label': '60 Seconds', 'value': 'WK'},
                                #{'label': '1 Month', 'value': 'MN'},
                                #{'label': '1 Year', 'value': 'YR'},
                            ],
                            value=['DY','WK','MN','YR'],
                            labelStyle={'display': 'inline-block'}
                    ),
                ],
                className='row',
                style={'margin-top': '20'}
        ),


        html.Div(
            [

                dcc.Graph(
                id='Moisture vs Time',
                figure={
                    'data': [
                        go.Scatter(
                            x=df[df['nodeID'] == i]['sec'][-10:],
                            y=df[df['nodeID'] == i]['moisture'][-10:],
                            mode = 'lines+markers',
                            opacity=0.7,
                            marker={
                                'size': 15,
                                'line': {'width': 0.5, 'color': 'white'}
                            },
                            name=str(i)
                        ) for i in df.nodeID.unique()
                    ],
                    'layout': go.Layout(
                        xaxis={'type': 'category', 'title': 'sec'},
                        yaxis={'title': 'Moisture'},
                        margin={'l': 50, 'b': 40, 't': 50, 'r': 10},
                        legend={'x': 1, 'y': 0},
                        hovermode='closest'
                    )
                }
            ),
                dcc.Graph(
                id='Location',
                figure={
                    'data': [
                        go.Scattergeo(
                            locationmode = 'USA-states',
                            lon = dfTable[dfTable['nodeID'] == i]['longitude'],
                            lat = dfTable[dfTable['nodeID'] == i]['latitude'],
                            text = dfMap['text'],
                            mode = 'markers',
                            marker = dict(
                                size = 8,
                                opacity = 0.8,
                                reversescale = True,
                                autocolorscale = False,
                                symbol = 'square',
                                line = dict(
                                    width=1,
                                    color='rgba(102, 102, 102)'
                                ),
                                colorscale = [ [0,"rgb(5, 10, 172)"],[0.35,"rgb(40, 60, 190)"],[0.5,"rgb(70, 100, 245)"],\
                                                [0.6,"rgb(90, 120, 245)"],[0.7,"rgb(106, 137, 247)"],[1,"rgb(220, 220, 220)"] ],
                                cmin = 0,
                                color = dfMap['cnt'],
                                cmax = dfMap['cnt'].max(),
                                colorbar=dict(
                                    title="Node Locations"
                                )
                        ))
                    ]#,
                    #'layout': go.Layout(
                        #xaxis={'title': 'Lattitude'}, #'type': 'category',
                        #yaxis={'title': 'Longitude'},
                        #margin={'l': 50, 'b': 40, 't': 50, 'r': 10},
                        #legend={'x': 1, 'y': 0},
                        #hovermode='closest'
                    #)
                }
            )


            ],
    className='six columns'
    ),


            #html.Div(children=[
                #html.H4(children='Soil Moisture'),
                #html.Table(
                    # Header
                #    [html.Tr([html.Th(col) for col in dataframe.columns])] +

                    # Body
                #    [html.Tr([
                #        html.Td(dataframe.iloc[i][col]) for col in dataframe.columns
                #    ]) for i in range(min(len(dataframe), max_rows))]
                #)
            #])




        html.Div(
                    [
                        dcc.Interval(
                            id='interval-component',
                            interval=1*1000, # in milliseconds
                            n_intervals=0
                        ),
                        dash_table.DataTable(
                            id='table',
                            columns=[{"name": i, "id": i} for i in dfTable.columns],
                            data=dfTable.to_dict("rows"),
                            style_table={
                                #'height': '200px',
                                #'overflowY': 'hidden',
                                'border': 'thin lightgrey solid'
                            },
                        )
                        #html.Table(

                            # Header
                        #    [html.Tr([html.Th(col) for col in df.columns])] +

                            # Body
                        #    [html.Tr([
                        #        html.Td(df.iloc[i][col]) for col in df.columns
                        #    ]) for i in range(min(len(df), 5))]
                        #)
                    ],
                    style = layout_table,
                    className="four columns"
                ),

        html.Div([
            #html.Div(dcc.Input(id='input-box', type='Update')),
            html.Button('Update', id='button'),
            #html.Div(id='output-container-button',
            #children='Enter a value and press submit')
             ],
             className = "row"
             )




                ],
                className="row"
        )


])


#@app.callback(
#    Output('table', 'data'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
#    #if n_clicks>0:
#    print(n_clicks)
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)

#    dfTable = pd.read_csv('the_new_csv.csv')
#    print(dfTable)
#    rows = dfTable.to_dict("rows")
#    return rows

#@app.callback(
#    Output('table', 'columns'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
    #if n_clicks>0:
#    print(n_clicks)
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)
#
#    dfTable = pd.read_csv('the_new_csv.csv')
#    print(dfTable)
#    columns=[{"name": i, "id": i} for i in dfTable.columns]
#    return columns

@app.callback(
    Output('table', 'data'),
    [Input('interval-component', 'n_intervals')])
def update_row_indices(n_intervals):
    df1 = pd.read_csv('NODE1.csv')
    df2 = pd.read_csv('NODE2.csv')
    df3 = pd.read_csv('NODE3.csv')
    #with open('WORK2.csv') as f:
    #    last3 = list(f)[-3:]
    
    #print(last3)
    
    #with open('created.csv', 'wb') as csvfile:
    #    filewriter = csv.writer(csvfile, delimiter = ',')
    #    filewriter.writerow(last3)
    
    
    
    data = [] #Buffer list
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reader:
            data.append(row)
            break
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)
                
    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)
                
    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i=0
        for row in reversed(list(csv.reader(input_file))):
            i+=1
            if i < 2:
                data.append(row)
    
    with open("the_new_csv.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        row = data[0]
        #print(row)
        writer.writerow(row)
    
        for new_row in data:
            if new_row[0] == '1':
               writer.writerow(new_row)
    
    
        for new_row in data:
            if new_row[0] == '2':
                writer.writerow(new_row)
    
        for new_row in data:
            if new_row[0] == '3':
                writer.writerow(new_row)
    
    data2 = [] #Buffer list
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)
                
    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i = 0
        for row in list(csv.reader(input_file)):
            if i != 0:
                data2.append(row)
            i+=1
                
    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i = 0
        for row in list(csv.reader(input_file)):
            if i != 0:
                data2.append(row)
            i+=1
            
            
    with open("the_new_csv2.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        for new_row in data2:
               writer.writerow(new_row)
    
    
    dfTable = pd.read_csv('the_new_csv.csv')
    df = pd.read_csv('the_new_csv2.csv')
    #print(dfTable)
    rows = dfTable.to_dict("rows")
    return rows


#    columns=[{"name": i, "id": i} for i in dfTable.columns]
#    return columns


#Needs fixed
@app.callback(
    Output('Moisture vs Time', 'figure'),
    [Input('TimeFrame', 'value'),
    Input('interval-component', 'n_intervals')])
def update_figure(value,n_intervals):
    data2 = [] #Buffer list
    with open("NODE1.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        for row in list(csv.reader(input_file)):
            data2.append(row)
                
    with open("NODE2.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i = 0
        for row in list(csv.reader(input_file)):
            if i != 0:
                data2.append(row)
            i+=1
                
    with open("NODE3.csv", "r") as input_file:
        reader = csv.reader(input_file, delimiter=',')
        i = 0
        for row in list(csv.reader(input_file)):
            if i != 0:
                data2.append(row)
            i+=1
            
            
    with open("the_new_csv2.csv", "w+") as to_file:
        writer = csv.writer(to_file, delimiter=',')
        for new_row in data2:
               writer.writerow(new_row)

    df = pd.read_csv('the_new_csv2.csv')

    if value == 'DY':
        data = [
            go.Scatter(
                x=df[df['nodeID'] == i]['sec'][-10:],
                y=df[df['nodeID'] == i]['moisture'][-10:],
                mode = 'lines+markers',
                opacity=0.7,
                marker={
                    'size': 15,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]
    else:
        data = [
            go.Scatter(
                x=df[df['nodeID'] == i]['sec'][-60:],
                y=df[df['nodeID'] == i]['moisture'][-60:],
                mode = 'lines+markers',
                opacity=0.7,
                marker={
                    'size': 15,
                    'line': {'width': 0.5, 'color': 'white'}
                },
                name=str(i)
            ) for i in df.nodeID.unique()
        ]


    return go.Figure(data=data)

@app.callback(
    Output('Location', 'figure'),
    [Input('interval-component', 'n_intervals')])
def update_figure(n_intervals):
    #df = pd.read_csv('WORK2.csv')

    data = [
        go.Scattergeo(
                            locationmode = 'USA-states',
                            lon = dfTable[dfTable['nodeID'] == i]['longitude'],
                            lat = dfTable[dfTable['nodeID'] == i]['latitude'],
                            text = dfMap['text'],
                            mode = 'markers',
                            marker = dict(
                                size = 8,
                                opacity = 0.8,
                                reversescale = True,
                                autocolorscale = False,
                                symbol = 'square',
                                line = dict(
                                    width=1,
                                    color='rgba(102, 102, 102)'
                                ),
                                colorscale = [ [0,"rgb(5, 10, 172)"],[0.35,"rgb(40, 60, 190)"],[0.5,"rgb(70, 100, 245)"],\
                                                [0.6,"rgb(90, 120, 245)"],[0.7,"rgb(106, 137, 247)"],[1,"rgb(220, 220, 220)"] ],
                                cmin = 0,
                                color = dfMap['cnt'],
                                cmax = dfMap['cnt'].max(),
                                colorbar=dict(
                                    title="Node Locations"
                                )
                        ))
    ]



    return go.Figure(data=data)


#@app.callback(
#    Output('table', 'columns'),
#    [Input('button', 'n_clicks')])
#def update_output(n_clicks):
#    df = pd.read_csv('WORK2.csv')
#
#    data = [] #Buffer list
#    with open("WORK2.csv", "rb") as input_file:
#        reader = csv.reader(input_file, delimiter=',')
#        i=0
#        for row in reader:
#            data.append(row)
#            break
#        for row in reversed(list(csv.reader(input_file))):
#            i+=1
#            if i < 5:
#                data.append(row)
#
#    with open("the_new_csv.csv", "w+") as to_file:
#        writer = csv.writer(to_file, delimiter=',')
#        for new_row in data:
#            writer.writerow(new_row)
#
#    dfTable2 = pd.read_csv('the_new_csv.csv')
#    columns=[{"name": i, "id": i} for i in dfTable2.columns]
#    return columns

#    return 'The input value was "{}" and the button has been clicked {} times'.format(
#        value,
#        n_clicks
#    )
#app.layout = html.Div([
#    dcc.Input(id='my-id', value='initial value', type='text'),
#    html.Div(id='my-div', children=df)
#])


#@app.callback(
#    Output(component_id='my-div', component_property='children'),
#    [Input(component_id='my-id', component_property='value')]
#)
#def update_output_div(input_value):
#    return 'You\'ve entered "{}"'.format(input_value)



if __name__ == '__main__':
    app.run_server(debug=True, port=8080)
