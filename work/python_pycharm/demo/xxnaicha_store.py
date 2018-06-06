
#奶茶
nc_list=["原味奶茶","芒果味奶茶","抹茶味奶茶"]

vip_num = [0,1,2,3,4] #记录会员号
info_custom = [] #记录当前消费信息
info_today_custom = []

cut_vip = 1 #这个值用于判断是否享用百分之20的优惠
            # 1为享有优惠，0为本次没有优惠
buy_car =[0] #第一位为标志位，之后为奶茶口味选择
while True:
    if len(info_today_custom)==20:
        print("今日已闭店，欢迎您明天光临")
        break
    customs_choice = input("")
    customs_num = input("请输入会员号")
    if int(customs_num) in vip_num:
        pass
    else:
        vip_num.append(vip_num[len(vip_num)-1]+1)
        cut_vip = 0

    while buy_car[0]==0:
        nc_chioce1 = input("请输入您要购买的奶茶口味：1.原味，2.芒果，3.抹茶")
        if int(nc_chioce1) == 1:
            buy_car.append(nc_list[0])
        elif int(nc_chioce1) == 2:
            buy_car.append(nc_list[1])
        elif int(nc_chioce1) == 3:
            buy_car.append(nc_list[2])
        else:
            print("您的输入有误请重新输入")
            continue
    while True:
        nc_chioce2 = input("请输入是否继续选择奶茶口味继续购买：输入y表示继续购买，输入n表示结束选择进行结算")
        if nc_chioce2 == y:
            buy_car[0]=0
            break
        elif nc_chioce2 == n:
            buy_car[0] = 1
            break
        else:
            print("输入无效")
            continue
    if len(buy_car)>1:
        print("您购买的商品如下",buy_car[1:])
    else:
        print("您没有选择任何口味的奶茶")
    if cut_vip == 0:
        print("本次将不享受会员价优惠，价格为",price)
    else:
        print("本次享受百分之20折扣优惠，价格为",price*0.8)
    info_custom.append(int(customs_num))
    info_custom.append(buy_car[1:])
    info_custom.append(cut_vip)
    print(info_custom) #观测消费记录是否正确
    info_today_custom.append(info_custom)



