# FrB Project

This repository contains two complementary efforts around the FrB language:

- `interpreter/` – the original C++ interpreter, toolchain, and regression suite that define the historical behaviour of FrB programs. See `interpreter/README.md` for build instructions, dependencies, and usage details.
- `pyfrb/` – the Python-based reimplementation that lexes, parses, optimizes, and lowers FrB programs directly to CPython bytecode. See `pyfrb/README.md` for information about running the compiler and executing regression tests.

## Example FrB Programs

### Simple English Program

```vb
DefLang EN

Public Class Main

    Public Shared Function Add(ByRef a As Int, ByRef b As Int) As Int
        Return a + b
    End Function

    Public Shared Sub main()
        Dim total As Int = Main.Add(2, 5)
        Debug.println("Total = " + total)
    End Sub

End Class
```

In the legacy interpreter, this code is compiled/executed by the C++ toolchain. In PyFrB, the same AST is lowered into CPython bytecode, and run directly in the Python VM.

### Linked List (EN)

```vb
DefLang EN


Public Class Main
    Public Shared Sub main()
        Dim list As Utils.LinkedList
        list.add(5)
        list.add(4)

        Dim it As Utils.LinkedList.Iterator := Nothing                
        Debug.println("List contains:")
        For it := list.begin() To list.end()
            Debug.println(" - " + Cast(Int, it.val()) )
        Next
    End Sub
End Class

Public Module Utils

    Public Class LinkedList
    
        Private Class Node
            Public next As Node := Nothing
            Public value As Object := Nothing
            
            Public Sub Initialize(n As Node, v As Object)
                next := n
                value := v
            End Sub

            Public Function reverse(prec As Node) As Node
                Dim n As Node := next
                next := prec

                If n Is Nothing Then
                    Return Me
                Else
                    Return n.reverse(Me)
                End If
            End Function
        End Class
        
        Public Class Iterator
            Private pos As Node := Nothing
            
            Private Shared end As Iterator := New Iterator(Nothing)
            
            Public Sub Initialize(n As Node)
                pos := n
            End Sub
            
            Public Operator++() As Iterator
                If pos Is Nothing Then Return end
                
                If pos.next Is Nothing Then
                    pos := Nothing
                    Return end
                Else
                    pos := pos.next
                    Return Me
                End If
            End Operator
            
            Public Operator<=(o As Iterator) As Bool
               Return Not Me.pos Is o.pos
            End Operator
            
            Public Function val() As Object
                If pos Is Nothing Then
                    Return Nothing
                Else
                    Return pos.value
                End If
            End Function
            
        End Class
    
        Protected root As Node := Nothing
        Protected lastNode As Node := Nothing
        Protected size As Int

        Public Sub add(v As Object)
            Dim n As Node := New Node(Nothing, v)
            
            If root Is Nothing Then
                root := n
            Else
                lastNode.next := n
            End If
            
            lastNode := n
            
            size++
            
        End Sub
        
        Public Sub addHead(v As Object)
            root := New Node(root, v)
            size++
        End Sub
        
        Public Sub reverse()
            If Not root Is Nothing Then
                lastNode := root
                root := root.reverse(Nothing)
            End If
        End Sub
        
        Public Function size() As Int
            Return size
        End Function
        
        Public Operator #() As Int
            Return size
        End Operator
        
        Public Operator+=(o As LinkedList) As LinkedList
            If #o == 0 Then Return Me
            
            Dim it As Iterator := o.begin()
            
            Dim n As Node := New Node(Nothing, it.val())
            
            If root Is Nothing Then
                root := n
            Else
                lastNode.next := n
            End If
            
            lastNode := n
            
            For it := it++ To o.end()
                lastNode := lastNode.next := New Node(Nothing, it.val())
            Next
            
            size += #o
            
            Return Me
        End Operator
        
        Public Function begin() As Iterator
            Return New Iterator(root)
        End Function
        
        Public Function end() As Iterator
            Return Iterator.end
        End Function
    
    End Class
    
End Module

```

### Simple French Program

```vb
DefLang FR

Publique Classe Main

    Publique Partagee Fonction addition(a Comme Entier, b Comme Entier) Comme Entier
        Retourne a + b
    Fin Fonction

    Publique Partagee Proc main()
        Soit somme Comme Entier := Main.addition(4, 7)
        Debug.println("Somme = " + somme)
    Fin Proc

Fin Classe

```

## Language Highlights

- Dual keyword sets via `DefLang EN` / `DefLang FR`
- Generics/templates for reusable type-safe components
- Events and delegates for publisher/subscriber style callbacks
- Operator overloading for custom numeric and container types
- Rich class system with shared/instance members and nested declarations

## Getting Started

1. **Legacy Interpreter:** refer to `interpreter/README.md`.
2. **Python Reimplementation:** refer to `pyfrb/README.md`.

